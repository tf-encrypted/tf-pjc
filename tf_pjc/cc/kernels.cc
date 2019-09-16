#include <memory>
#include <string>
#include <vector>

#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/framework/resource_mgr.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/variant.h"
#include "tensorflow/core/framework/variant_encode_decode.h"
#include "tensorflow/core/framework/variant_op_registry.h"
#include "tensorflow/core/framework/variant_tensor_data.h"
#include "tensorflow/core/lib/core/errors.h"

#include "private_join_and_compute.h"

using namespace tensorflow;  // NOLINT

template <typename E>
std::string convert_to_string(E x) {
  return std::to_string(x);
}

template <>
std::string convert_to_string(std::string x) {
  return x;
}

template <typename E, typename V>
class PjcRunClientOp : public OpKernel {
 public:
  explicit PjcRunClientOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override { 
    // TODO(Morten) avoid these extra copies; pass iterator directly into PJC?

    // read elements input
    const Tensor& elements_tensor = ctx->input(0);
    auto elements_flat = elements_tensor.flat<E>();
    std::vector<std::string> elements;
    elements.reserve(elements_flat.size());
    for (int i = 0; i < elements_flat.size(); i++) {
      std::string element = convert_to_string<E>(elements_flat.data()[i]);
      elements.push_back(element);
    }

    // read values input
    const Tensor& values_tensor = ctx->input(1);
    auto values_flat = values_tensor.flat<V>();
    std::vector<int64_t> values;
    values.reserve(values_flat.size());
    for (int i = 0; i < values_flat.size(); i++) {
      int64_t value = static_cast<int64_t>(values_flat.data()[i]); // TODO(Morten) explicit cast
      values.push_back(value);
    }

    // run
    ::private_join_and_compute::ClientResult result;
    ::private_join_and_compute::ClientSession session; 
    int res = session.Run(1536,
                          "0.0.0.0:10501",
                          std::move(elements),
                          std::move(values),
                          &result);
    OP_REQUIRES(ctx, res == 0, errors::Unknown("Session failed"));

    // write size output
    Tensor* size_tensor = nullptr;
    OP_REQUIRES_OK(ctx, ctx->allocate_output(0, TensorShape{}, &size_tensor));
    size_tensor->scalar<int64_t>()() = result.intersection_size;
    
    // write sum output
    Tensor* sum_tensor = nullptr;
    OP_REQUIRES_OK(ctx, ctx->allocate_output(1, TensorShape{}, &sum_tensor));
    sum_tensor->scalar<int64_t>()() = static_cast<int64_t>(result.intersection_sum); // TODO(Morten) check bounds?
  }
};

struct ServerResource : ResourceBase {
  ::private_join_and_compute::ServerSession session;

  std::string DebugString() const override { 
    return "<server session>";
  }
};

template <typename E>
class PjcRunServerOp : public OpKernel {
 public:
  explicit PjcRunServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    auto resource_mgr = ctx->resource_manager();

    // read input
    const Tensor& elements_tensor = ctx->input(0);
    auto elements_flat = elements_tensor.flat<E>();
    std::vector<std::string> elements;
    elements.reserve(elements_flat.size());
    for (int i = 0; i < elements_flat.size(); i++) {
      std::string element = convert_to_string<E>(elements_flat.data()[i]);
      elements.push_back(element);
    }

    // run
    ServerResource* server_resource = new ServerResource;
    OP_REQUIRES_OK(ctx, resource_mgr->Create("pjc", "server", server_resource));
    server_resource->session.Run("0.0.0.0:10501", std::move(elements));

    // cleanup
    OP_REQUIRES_OK(ctx, resource_mgr->Delete<ServerResource>("pjc", "server"));
    server_resource = nullptr;
  }
};

template <typename E>
class PjcRunAsyncServerOp : public OpKernel {
 public:
  explicit PjcRunAsyncServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    auto resource_mgr = ctx->resource_manager();
    
    // read input
    const Tensor& elements_tensor = ctx->input(0);
    auto elements_flat = elements_tensor.flat<E>();
    std::vector<std::string> elements;
    elements.reserve(elements_flat.size());
    for (int i = 0; i < elements_flat.size(); i++) {
      std::string element = convert_to_string<E>(elements_flat.data()[i]);
      elements.push_back(element);
    }

    // run
    ServerResource* server_resource = new ServerResource;
    OP_REQUIRES_OK(ctx, resource_mgr->Create("pjc", "server", server_resource));
    server_resource->session.RunAsync("0.0.0.0:10501", std::move(elements));
  }
};

class PjcWaitServerOp : public OpKernel {
 public:
  explicit PjcWaitServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    auto resource_mgr = ctx->resource_manager();
    
    // wait until server is shut down
    ServerResource* server_resource = nullptr;
    OP_REQUIRES_OK(ctx, resource_mgr->Lookup("pjc", "server", &server_resource));
    server_resource->session.Wait();

    // cleanup
    OP_REQUIRES_OK(ctx, resource_mgr->Delete<ServerResource>("pjc", "server"));
    server_resource = nullptr;
  }
};

REGISTER_KERNEL_BUILDER(Name("PjcRunClient").Device(DEVICE_CPU).TypeConstraint<string>("elements_dtype").TypeConstraint<int64>("values_dtype"), PjcRunClientOp<std::string, int64_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunClient").Device(DEVICE_CPU).TypeConstraint<string>("elements_dtype").TypeConstraint<int32>("values_dtype"), PjcRunClientOp<std::string, int32_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunClient").Device(DEVICE_CPU).TypeConstraint<int64>("elements_dtype").TypeConstraint<int64>("values_dtype"), PjcRunClientOp<int64_t, int64_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunClient").Device(DEVICE_CPU).TypeConstraint<int64>("elements_dtype").TypeConstraint<int32>("values_dtype"), PjcRunClientOp<int64_t, int32_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunClient").Device(DEVICE_CPU).TypeConstraint<int32>("elements_dtype").TypeConstraint<int64>("values_dtype"), PjcRunClientOp<int32_t, int64_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunClient").Device(DEVICE_CPU).TypeConstraint<int32>("elements_dtype").TypeConstraint<int32>("values_dtype"), PjcRunClientOp<int32_t, int32_t>);

REGISTER_KERNEL_BUILDER(Name("PjcRunServer").Device(DEVICE_CPU).TypeConstraint<string>("elements_dtype"), PjcRunServerOp<std::string>);
REGISTER_KERNEL_BUILDER(Name("PjcRunServer").Device(DEVICE_CPU).TypeConstraint<int64>("elements_dtype"), PjcRunServerOp<int64_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunServer").Device(DEVICE_CPU).TypeConstraint<int32>("elements_dtype"), PjcRunServerOp<int32_t>);

REGISTER_KERNEL_BUILDER(Name("PjcRunAsyncServer").Device(DEVICE_CPU).TypeConstraint<string>("elements_dtype"), PjcRunAsyncServerOp<std::string>);
REGISTER_KERNEL_BUILDER(Name("PjcRunAsyncServer").Device(DEVICE_CPU).TypeConstraint<int64>("elements_dtype"), PjcRunAsyncServerOp<int64_t>);
REGISTER_KERNEL_BUILDER(Name("PjcRunAsyncServer").Device(DEVICE_CPU).TypeConstraint<int32>("elements_dtype"), PjcRunAsyncServerOp<int32_t>);

REGISTER_KERNEL_BUILDER(Name("PjcWaitServer").Device(DEVICE_CPU), PjcWaitServerOp);
