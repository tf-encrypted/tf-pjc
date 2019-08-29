licenses(["notice"])  # Apache 2.0

package(default_visibility = ["//visibility:public"])

cc_binary(
    name = 'python/_psi_ops.so',
    srcs = [
        "cc/ops.cc",
        "cc/kernels.cc",
    ],
    linkshared = 1,
    deps = [
        # "@com_github_google_private_join_and_compute//:server_impl",
        # "@com_github_google_private_join_and_compute//:data_util",
        # "@com_github_google_private_join_and_compute//:private_join_and_compute_rpc_impl",
        # "@com_google_absl//absl/synchronization",
        # "@com_google_absl//absl/time",
        "@com_github_glog_glog//:glog",
        "@com_github_grpc_grpc//:grpc",
        "@com_github_grpc_grpc//:grpc++",
        "@local_config_tf//:libtensorflow_framework",
        "@local_config_tf//:tf_header_lib",
    ],
    copts = ["-pthread", "-std=c++11", "-D_GLIBCXX_USE_CXX11_ABI=0", "-fPIC"],
)

py_library(
    name = "psi_ops_py",
    srcs = ([
        "python/ops.py",
    ]),
    data = [
        ":python/_psi_ops.so"
    ],
    srcs_version = "PY3",
)

py_test(
    name = "psi_ops_py_test",
    srcs = [
        "python/ops_test.py",
    ],
    main = "python/ops_test.py",
    deps = [
        ":psi_ops_py",
    ],
    srcs_version = "PY3",
)
