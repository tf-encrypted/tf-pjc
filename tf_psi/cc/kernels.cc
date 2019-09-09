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

#include "private_join_and_compute.h"

using namespace tensorflow;  // NOLINT

class PsiRunClientOp : public OpKernel {
 public:
  explicit PsiRunClientOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    std::vector<std::string> elements { "a", "b", "c" };
    std::vector<int64_t> values { 1, 2, 3 };

    ::private_join_and_compute::ClientSession session;
    ::private_join_and_compute::ClientResult result;
    int res = session.Run(1536, "0.0.0.0:10501", std::move(elements), std::move(values), &result);
  }
};

struct ServerResource : ResourceBase {
  ::private_join_and_compute::ServerSession session;

  std::string DebugString() const override { 
    return "<server session>";
  }
};

class PsiRunServerOp : public OpKernel {
 public:
  explicit PsiRunServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    auto resource_mgr = ctx->resource_manager();

    ServerResource* server_resource = new ServerResource;
    OP_REQUIRES_OK(ctx, resource_mgr->Create("psi", "server", server_resource));

    std::vector<std::string> elements { "a", "c", "d" };

    server_resource->session.Run("0.0.0.0:10501", std::move(elements));
  }
};

class PsiRunAsyncServerOp : public OpKernel {
 public:
  explicit PsiRunAsyncServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    auto resource_mgr = ctx->resource_manager();

    ServerResource* server_resource = new ServerResource;
    OP_REQUIRES_OK(ctx, resource_mgr->Create("psi", "server", server_resource));

    std::vector<std::string> elements { "a", "c", "d" };

    server_resource->session.RunAsync("0.0.0.0:10501", std::move(elements));
  }
};

class PsiWaitServerOp : public OpKernel {
 public:
  explicit PsiWaitServerOp(OpKernelConstruction* ctx) : OpKernel(ctx) {}

  void Compute(OpKernelContext* ctx) override {
    auto resource_mgr = ctx->resource_manager();
    
    ServerResource* server_resource = nullptr;
    OP_REQUIRES_OK(ctx, resource_mgr->Lookup("psi", "server", &server_resource));

    server_resource->session.Wait();
  }
};

REGISTER_KERNEL_BUILDER(Name("PsiRunClient").Device(DEVICE_CPU), PsiRunClientOp);
REGISTER_KERNEL_BUILDER(Name("PsiRunServer").Device(DEVICE_CPU), PsiRunServerOp);
REGISTER_KERNEL_BUILDER(Name("PsiRunAsyncServer").Device(DEVICE_CPU), PsiRunAsyncServerOp);
REGISTER_KERNEL_BUILDER(Name("PsiWaitServer").Device(DEVICE_CPU), PsiWaitServerOp);
