#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/common_shape_fns.h"

REGISTER_OP("PsiLaunchServer")
    .Input("elements: int32")
    .SetIsStateful();

REGISTER_OP("PsiWaitServer")
    .SetIsStateful();
    // .SetShapeFn(::tensorflow::shape_inference::UnchangedShape);

REGISTER_OP("PsiLaunchClient")
    .Input("elements: string")
    .Input("values: int32")
    .Output("sum: int32")
    .SetIsStateful();
//     // .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
//     //     ::tensorflow::shape_inference::ShapeHandle val0 = c->input(0);
//     //     ::tensorflow::shape_inference::ShapeHandle val1 = c->input(1);
//     //     ::tensorflow::shape_inference::ShapeHandle res;
//     //     TF_RETURN_IF_ERROR(c->Merge(val0, val1, &res));
//     //     c->set_output(0, res);
//     //     return ::tensorflow::Status::OK();
//     // });
