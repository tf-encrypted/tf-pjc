import tensorflow
from tensorflow.python.framework import load_library
from tensorflow.python.framework.errors import NotFoundError
from tensorflow.python.platform import resource_loader


op_lib_file = resource_loader.get_path_to_datafile('_psi_ops.so')
psi_ops = load_library.load_op_library(op_lib_file)

psi_launch_server = psi_ops.psi_launch_server
