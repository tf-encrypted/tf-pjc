#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"

REGISTER_OP("PjcRunClient")
    .Attr("elements_dtype: {string, int64, int32}")
    .Attr("values_dtype: {int64, int32}")
    .Input("elements: elements_dtype")
    .Input("values: values_dtype")
    .Output("size: int64")
    .Output("sum: int64")
    .SetIsStateful()
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
        ::tensorflow::shape_inference::ShapeHandle elements = c->input(0);
        ::tensorflow::shape_inference::ShapeHandle values = c->input(1);
        ::tensorflow::shape_inference::ShapeHandle unused;
        TF_RETURN_IF_ERROR(c->Merge(elements, values, &unused));

        ::tensorflow::shape_inference::ShapeHandle size = c->Scalar();
        ::tensorflow::shape_inference::ShapeHandle sum = c->Scalar();
        c->set_output(0, size);
        c->set_output(1, sum);

        return ::tensorflow::Status::OK();
    });

REGISTER_OP("PjcRunServer")
    .Attr("elements_dtype: {string, int64, int32}")
    .Input("elements: elements_dtype")
    .SetIsStateful();

REGISTER_OP("PjcRunAsyncServer")
    .Attr("elements_dtype: {string, int64, int32}")
    .Input("elements: elements_dtype")
    .SetIsStateful();

REGISTER_OP("PjcWaitServer")
    .SetIsStateful();
