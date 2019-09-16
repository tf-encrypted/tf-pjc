import tensorflow as tf

from tf_pjc.python.ops import pjc_run_client
from tf_pjc.python.ops import pjc_run_server
from tf_pjc.python.ops import pjc_run_async_server
from tf_pjc.python.ops import pjc_wait_server


class PrivateIntersectionSum:

  def __init__(self, client_device, server_device):
    self.client_device = client_device
    self.server_device = server_device

  def __call__(self, client_elements, client_values, server_elements, wait_for_server=True):
    assert isinstance(server_elements, tf.Tensor)
    assert isinstance(client_elements, tf.Tensor)
    assert isinstance(client_values, tf.Tensor)
    assert client_elements.shape == client_values.shape

    # launch server and wait
    with tf.device(self.server_device):
      server_run_async_op = pjc_run_async_server(elements=server_elements)

      with tf.control_dependencies([server_run_async_op]):
        server_wait_op = pjc_wait_server()

    # once server is launched we can run the client,
    # using TensorFlow for the synchronization
    with tf.device(self.client_device):
      with tf.control_dependencies([server_run_async_op]):
        client_run_op = pjc_run_client(elements=client_elements,
                                       values=client_values)

    if not wait_for_server:
      return client_run_op, server_run_async_op

    return client_run_op, server_wait_op
