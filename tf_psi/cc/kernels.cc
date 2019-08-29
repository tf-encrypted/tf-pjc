#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/resource_mgr.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/variant.h"
#include "tensorflow/core/framework/variant_encode_decode.h"
#include "tensorflow/core/framework/variant_op_registry.h"
#include "tensorflow/core/framework/variant_tensor_data.h"

// #include "tensorflow/core/lib/core/refcount.h"





// #include <iostream>
// #include <memory>
// #include <string>
// #include <thread>  // NOLINT

// #include "include/grpc/grpc_security_constants.h"
// #include "include/grpcpp/grpcpp.h"
// #include "include/grpcpp/security/server_credentials.h"
// #include "include/grpcpp/server_builder.h"
// #include "include/grpcpp/server_context.h"
// #include "include/grpcpp/support/status.h"

// #include "absl/time/time.h"
// #include "absl/synchronization/notification.h"

// #include "data_util.h"
// #include "server_impl.h"
// #include "private_join_and_compute.grpc.pb.h"
// #include "private_join_and_compute_rpc_impl.h"
// #include "protocol_server.h"

// struct ServerResource {
//   absl::Notification protocol_finished;
//   ::private_join_and_compute::Context context;
//   std::unique_ptr<::grpc::Server> grpc_server;
//   std::unique_ptr<::private_join_and_compute::PrivateJoinAndComputeRpcImpl> service;
// };




using namespace tensorflow;  // NOLINT

class PsiLaunchServerOp : public OpKernel {
 public:
  explicit PsiLaunchServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    const Tensor& elements = ctx->input(0);
    OP_REQUIRES(ctx,
        TensorShapeUtils::IsVector(elements.shape()),
        errors::InvalidArgument(
            "Input expected to be a vector but got shape: ",
            elements.shape().DebugString()));
    
    auto resource_mgr = ctx->resource_manager();
    
    // ServerResource* resource = new ServerResource;
    // OP_REQUIRES_OK(ctx, resource_mgr->Create("psi", "server", resource));

    // {
    // std::unique_ptr<::private_join_and_compute::ProtocolServer> server =
    //     absl::make_unique<::private_join_and_compute::PrivateIntersectionSumProtocolServerImpl>(
    //         &resource->context,
    //         &resource->protocol_finished,
    //         std::move(maybe_server_identifiers.ValueOrDie())
    //   );
    // resource->service =
    //     absl::make_unique<::private_join_and_compute::PrivateJoinAndComputeRpcImpl>(std::move(server));

    // ::grpc::ServerBuilder builder;
    // // Consider grpc::SslServerCredentials if not running locally.
    // builder.AddListeningPort(port, ::grpc::experimental::LocalServerCredentials(grpc_local_connect_type::LOCAL_TCP));
    // builder.RegisterService(resource->service.get());

    // resource->grpc_server = builder.BuildAndStart();
    // }




    // auto elements_raw = input_tensor.flat<T>().data();

    // ClientResource * res2 = nullptr;
    // Status s2 = resource_mgr->Lookup("connections", res_name, &res2);
    // OP_REQUIRES_OK(context, s2);



    // auto res = new ClientResource;
    // res->client = ModelClient(grpc::CreateCustomChannel("localhost:50051",
    //                           grpc::InsecureChannelCredentials(), ch_args));
    // auto resource_mgr = context->resource_manager();
    // Status s = resource_mgr->Create("psi", "client", res);
    // OP_REQUIRES_OK(context, s);
  }
};

REGISTER_KERNEL_BUILDER(Name("PsiLaunchServer").Device(DEVICE_CPU), PsiLaunchServerOp);
