.bazelrc:
	TF_NEED_CUDA=0 ./configure.sh

clean:
	bazel clean
	rm -f .bazelrc

test: .bazelrc
	bazel test ... --test_output=all

build: .bazelrc
	bazel build "//tf_psi:psi_ops_py"

.PHONY: clean test build
