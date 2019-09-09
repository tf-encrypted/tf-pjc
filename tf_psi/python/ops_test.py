import tensorflow as tf
from tensorflow.python.platform import test

from tf_psi.python.ops import psi_run_client
from tf_psi.python.ops import psi_run_server
from tf_psi.python.ops import psi_run_async_server
from tf_psi.python.ops import psi_wait_server

class PsiTest(test.TestCase):

  #  def test_run_client(self):
  #    # NOTE: standalone server must be launched for this test
  #   client_elements = tf.constant(["a", "b", "c"])
  #   client_values = tf.constant([1, 2, 3])
  #   client_run_op = psi_run_client(
  #       elements=client_elements,
  #       values=client_values)
  #   with tf.Session() as sess:
  #     client_result = sess.run([client_run_op])
  #     assert True

  # def test_run_server(self):
  #   # NOTE: standalone client must be launched for this test
  #   server_elements = tf.constant(["b", "c"])
  #   server_run_op = psi_run_server(server_elements)
  #   with tf.Session() as sess:
  #     sess.run([server_run_op])
  #     assert True

  # def test_run_async_server(self):
  #   # NOTE: standalone client must be launched for this test
  #   server_elements = tf.constant(["b", "c"])
  #   server_run_async_op = psi_run_async_server(server_elements)
  #   with tf.control_dependencies([server_run_async_op]):
  #     server_wait_op = psi_wait_server()
  #   with tf.Session() as sess:
  #     sess.run([server_wait_op])
  #     assert True

  def test_run(self):
    # inputs
    server_elements = tf.constant(["b", "c"])
    client_elements = tf.constant(["a", "b", "c"])
    client_values = tf.constant([1, 2, 3])
    # protocol
    server_run_async_op = psi_run_async_server(server_elements)
    with tf.control_dependencies([server_run_async_op]):
      client_run_op = psi_run_client(
          elements=client_elements,
          values=client_values)
      server_wait_op = psi_wait_server()
    # run
    with tf.Session() as sess:
      client_result, _ = sess.run([client_run_op, server_wait_op])
      print(client_result)
      assert True

if __name__ == '__main__':
  test.main()
