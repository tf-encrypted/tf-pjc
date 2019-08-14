load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "com_google_googletest",
    url = "https://github.com/google/googletest/archive/release-1.8.1.zip",
    strip_prefix = "googletest-release-1.8.1",
    sha256 = "927827c183d01734cc5cfef85e0ff3f5a92ffe6188e0d18e909c5efebf28a0c7",
)

#
# TensorFlow
#

load("//external/tf:tf_configure.bzl", "tf_configure")

tf_configure(name = "local_config_tf")

#
# Google Private Join and Compute
#

git_repository(
    name = "com_github_google_private_join_and_compute",
    remote = "https://github.com/mortendahl/private-join-and-compute.git",
    branch = "master",
)

# local_repository(
#     name = "com_github_google_private_join_and_compute",
#     path = "../private-join-and-compute",
# )

#
# Transitive dependencies for Google Private Join and Compute
#

git_repository(
    name = "com_google_protobuf",
    remote = "https://github.com/google/protobuf.git",
    tag = "v3.6.1.3",
)

http_archive(
    name = "com_github_glog_glog",
    build_file = "@//external/glog:BUILD",
    urls = ["https://github.com/google/glog/archive/v0.3.5.tar.gz"],
    strip_prefix = "glog-0.3.5",
)

# gflags
# Needed for glog
git_repository(
    name = "com_github_gflags_gflags",
    remote = "https://github.com/gflags/gflags.git",
    tag = "v2.2.2",
)

# BoringSSL
http_archive(
    name = "boringssl",
    urls = [
        "https://mirror.bazel.build/github.com/google/boringssl/archive/a0fb951d2a26a8ee746b52f3ba81ab011a0af778.tar.gz",
        "https://github.com/google/boringssl/archive/a0fb951d2a26a8ee746b52f3ba81ab011a0af778.tar.gz",
    ],
    sha256 = "524ba98a56300149696481b4cb9ddebd0c7b7ac9b9f6edee81da2d2d7e5d2bb3",
    strip_prefix = "boringssl-a0fb951d2a26a8ee746b52f3ba81ab011a0af778",
)

# Abseil C++ libraries
git_repository(
    name = "com_google_absl",
    remote = "https://github.com/abseil/abseil-cpp.git",
    tag = "20181200",
)

# gRPC
git_repository(
    name = "com_github_grpc_grpc",
    remote = "https://github.com/grpc/grpc.git",
    tag = "v1.19.0",
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()