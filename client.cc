/*
 * Copyright 2019 Google Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <iostream>
#include <memory>
#include <string>

#include "include/grpc/grpc_security_constants.h"
#include "include/grpcpp/channel.h"
#include "include/grpcpp/client_context.h"
#include "include/grpcpp/create_channel.h"
#include "include/grpcpp/grpcpp.h"
#include "include/grpcpp/security/credentials.h"
#include "include/grpcpp/support/status.h"
#include "data_util.h"
#include "client_impl.h"
#include "private_join_and_compute.grpc.pb.h"
#include "private_join_and_compute.pb.h"
#include "protocol_client.h"
#include "util/status.inc"
#include "absl/memory/memory.h"
#include "absl/strings/str_cat.h"

namespace private_join_and_compute {
namespace {

class InvokeServerHandleClientMessageSink : public MessageSink<ClientMessage> {
 public:
  explicit InvokeServerHandleClientMessageSink(
      std::unique_ptr<PrivateJoinAndComputeRpc::Stub> stub)
      : stub_(std::move(stub)) {}

  ~InvokeServerHandleClientMessageSink() override = default;

  Status Send(const ClientMessage& message) override {
    ::grpc::ClientContext client_context;
    ::grpc::Status grpc_status =
        stub_->Handle(&client_context, message, &last_server_response_);
    if (grpc_status.ok()) {
      return OkStatus();
    } else {
      return InternalError(absl::StrCat(
          "GrpcClientMessageSink: Failed to send message, error code: ",
          grpc_status.error_code(),
          ", error_message: ", grpc_status.error_message()));
    }
  }

  const ServerMessage& last_server_response() { return last_server_response_; }

 private:
  std::unique_ptr<PrivateJoinAndComputeRpc::Stub> stub_;
  ServerMessage last_server_response_;
};

int ExecuteProtocol(Context* context,
                    int32_t paillier_modulus_size,
                    std::string port,
                    const std::vector<std::string>& elements,
                    const std::vector<BigNum>& values) {

  std::cout << "Client: Generating keys..." << std::endl;
  std::unique_ptr<::private_join_and_compute::ProtocolClient> client =
      absl::make_unique<::private_join_and_compute::PrivateIntersectionSumProtocolClientImpl>(
          context,
          elements,
          values,
          paillier_modulus_size);

  // Consider grpc::SslServerCredentials if not running locally.
  std::cout << "Client: Connecting..." << std::endl;
  auto channel = ::grpc::CreateChannel(port,
    ::grpc::experimental::LocalCredentials(grpc_local_connect_type::LOCAL_TCP));
  std::unique_ptr<PrivateJoinAndComputeRpc::Stub> stub = PrivateJoinAndComputeRpc::NewStub(channel);
  InvokeServerHandleClientMessageSink message_sink(std::move(stub));

  // Execute StartProtocol and wait for response from ServerRoundOne.
  auto start_protocol_status = client->StartProtocol(&message_sink);
  if (!start_protocol_status.ok()) {
    return 1;
  }
  ServerMessage server_round_one = message_sink.last_server_response();

  // Execute ClientRoundOne, and wait for response from ServerRoundTwo.
  auto client_round_one_status = client->Handle(server_round_one, &message_sink);
  if (!client_round_one_status.ok()) {
    return 1;
  }

  // Execute ServerRoundTwo.
  ServerMessage server_round_two = message_sink.last_server_response();

  // Compute the intersection size and sum.
  auto intersection_size_and_sum_status = client->Handle(server_round_two, &message_sink);
  if (!intersection_size_and_sum_status.ok()) {
    std::cerr << "Client::ExecuteProtocol: failed to DecryptSum: "
              << intersection_size_and_sum_status << std::endl;
    return 1;
  }

  // Output the result.
  auto client_print_output_status = client->PrintOutput();
  if (!client_print_output_status.ok()) {
    std::cerr << "Client::ExecuteProtocol: failed to PrintOutput: "
              << client_print_output_status << std::endl;
    return 1;
  }

  return 0;
}

}  // namespace
}  // namespace private_join_and_compute

int main(int argc, char** argv) {
  google::InitGoogleLogging(argv[0]);

  ::private_join_and_compute::Context context;

  std::cout << "Client: Loading data..." << std::endl;
  auto maybe_client_data = ::private_join_and_compute::ReadClientDatasetFromFile("/tmp/dummy_client_data.csv", &context);
  if (!maybe_client_data.ok()) {
    return 1;
  }
  auto client_data = std::move(maybe_client_data.ValueOrDie());

  return private_join_and_compute::ExecuteProtocol(
      &context,
      1536,
      "0.0.0.0:10501",
      std::move(client_data.first),
      std::move(client_data.second)
  );
}
