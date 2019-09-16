import tensorflow as tf
from tensorflow.python.platform import test

from tf_pjc.python.ops import pjc_run_client
from tf_pjc.python.ops import pjc_run_server
from tf_pjc.python.ops import pjc_run_async_server
from tf_pjc.python.ops import pjc_wait_server

class PjcTest(test.TestCase):

  def test_run_on_strings(self):
    # inputs
    server_elements = tf.constant(["a", "b", "d"])
    client_elements = tf.constant(["a", "b", "c"])
    client_values = tf.constant([10, 20, 40], dtype=tf.int64)

    # protocol
    server_run_async_op = pjc_run_async_server(server_elements)
    with tf.control_dependencies([server_run_async_op]):
      client_run_op = pjc_run_client(
          elements=client_elements,
          values=client_values)
      server_wait_op = pjc_wait_server()

    # run
    with tf.Session() as sess:
      (intersection_size, intersection_sum), _ = sess.run([client_run_op, server_wait_op])

      assert intersection_size == 2, (intersection_size, intersection_sum)
      assert intersection_sum == 30, (intersection_size, intersection_sum)

  def test_run_on_int(self):
    # inputs
    server_elements = tf.constant([1, 2, 4])
    client_elements = tf.constant([1, 2, 3])
    client_values = tf.constant([10, 20, 40], dtype=tf.int64)

    # protocol
    server_run_async_op = pjc_run_async_server(server_elements)
    with tf.control_dependencies([server_run_async_op]):
      client_run_op = pjc_run_client(
          elements=client_elements,
          values=client_values)
      server_wait_op = pjc_wait_server()

    # run
    with tf.Session() as sess:
      (intersection_size, intersection_sum), _ = sess.run([client_run_op, server_wait_op])

      assert intersection_size == 2, (intersection_size, intersection_sum)
      assert intersection_sum == 30, (intersection_size, intersection_sum)


if __name__ == '__main__':
  test.main()
