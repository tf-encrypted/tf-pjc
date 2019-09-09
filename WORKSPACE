load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

http_archive(
    name = "com_google_googletest",
    url = "https://github.com/google/googletest/archive/release-1.8.1.zip",
    strip_prefix = "googletest-release-1.8.1",
    sha256 = "927827c183d01734cc5cfef85e0ff3f5a92ffe6188e0d18e909c5efebf28a0c7",
)

#
# Google Private Join and Compute (and transitive dependencies)
#

# git_repository(
#     name = "com_github_google_private_join_and_compute",
#     remote = "https://github.com/google/private-join-and-compute.git",
#     branch = "master",
# )

local_repository(
    name = "com_github_google_private_join_and_compute",
    path = "/Users/dahl/work/mortendahl/private-join-and-compute",
)

http_archive(
    name = "com_github_glog_glog",
    build_file = "@//third_party/glog:BUILD",
    urls = ["https://github.com/google/glog/archive/v0.3.5.tar.gz"],
    strip_prefix = "glog-0.3.5",
)

# com_google_absl used by TensorFlow
http_archive(
    name = "com_google_absl",
    # build_file = "//external:com_google_absl.BUILD",
    sha256 = "56cd3fbbbd94468a5fff58f5df2b6f9de7a0272870c61f6ca05b869934f4802a",
    strip_prefix = "abseil-cpp-daf381e8535a1f1f1b8a75966a74e7cca63dee89",
    urls = [
        "http://mirror.tensorflow.org/github.com/abseil/abseil-cpp/archive/daf381e8535a1f1f1b8a75966a74e7cca63dee89.tar.gz",
        "https://github.com/abseil/abseil-cpp/archive/daf381e8535a1f1f1b8a75966a74e7cca63dee89.tar.gz",
    ],
)

git_repository(
    name = "com_github_grpc_grpc",
    remote = "https://github.com/grpc/grpc.git",
    tag = "v1.19.0",
)

load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()

#
# TensorFlow
#

load("//third_party/tf:tf_configure.bzl", "tf_configure")
tf_configure(name = "local_config_tf")
