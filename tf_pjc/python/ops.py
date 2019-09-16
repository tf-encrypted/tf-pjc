import tensorflow
from tensorflow.python.framework import load_library
from tensorflow.python.framework.errors import NotFoundError
from tensorflow.python.platform import resource_loader


op_lib_file = resource_loader.get_path_to_datafile('_pjc_ops.so')
pjc_ops = load_library.load_op_library(op_lib_file)

pjc_run_client = pjc_ops.pjc_run_client
pjc_run_server = pjc_ops.pjc_run_server
pjc_run_async_server = pjc_ops.pjc_run_async_server
pjc_wait_server = pjc_ops.pjc_wait_server
