import tensorflow as tf
from tensorflow.python.platform import test

from tf_pjc.python.protocol import PrivateIntersectionSum

class PjcTest(test.TestCase):

  def test_simple(self):

    # players
    client_device = "/job:localhost/replica:0/task:0/device:CPU:0"
    server_device = "/job:localhost/replica:0/task:0/device:CPU:0"

    # inputs
    server_elements = tf.constant(["a", "b", "c"])
    client_elements = tf.constant(["a", "b", "c", "d"])
    client_values = tf.constant([10, 20, 40, 80])

    # protocol
    protocol = PrivateIntersectionSum(client_device, server_device)
    client_op, server_op = protocol(client_elements, client_values, server_elements)

    # run
    with tf.Session() as sess:
      (intersection_size, intersection_sum), _ = sess.run([client_op, server_op])

      assert intersection_size == 3, intersection_size
      assert intersection_sum == 70, intersection_sum

  # def test_stress(self):

  #   # players
  #   client_device = "/job:localhost/replica:0/task:0/device:CPU:0"
  #   server_device = "/job:localhost/replica:0/task:0/device:CPU:0"

  #   N = 1000

  #   # inputs
  #   server_elements = tf.range(N)
  #   client_elements = tf.range(N)
  #   client_values = tf.range(N)

  #   # protocol
  #   protocol = PrivateIntersectionSum(client_device, server_device)
  #   client_op, server_op = protocol(client_elements, client_values, server_elements)

  #   # run
  #   with tf.Session() as sess:
  #     (intersection_size, intersection_sum), _ = sess.run([client_op, server_op])

  #     assert intersection_size == N, (intersection_size, intersection_sum)
  #     assert intersection_sum == (N * (N - 1)) // 2, intersection_sum

if __name__ == '__main__':
  test.main()
