cc_binary(
    name = "client",
    srcs = ["client.cc"],
    deps = [
        "@com_github_google_private_join_and_compute//:client_impl",
        "@com_github_google_private_join_and_compute//:data_util",
        "@com_github_grpc_grpc//:grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)

cc_binary(
    name = "server",
    srcs = ["server.cc"],
    deps = [
        "@com_github_google_private_join_and_compute//:server_impl",
        "@com_github_google_private_join_and_compute//:data_util",
        "@com_github_google_private_join_and_compute//:private_join_and_compute_rpc_impl",
        "@com_google_absl//absl/synchronization",
        "@com_google_absl//absl/time",
        "@com_github_glog_glog//:glog",
        "@com_github_grpc_grpc//:grpc",
        "@com_github_grpc_grpc//:grpc++",
    ],
)
