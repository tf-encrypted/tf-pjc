import tensorflow as tf
from tensorflow.python.platform import test

from tf_pjc.python.protocol import PrivateIntersectionSum

class PjcTest(test.TestCase):

  def test_simple(self):

    # device strings of the two players involved
    client_device = "/job:localhost/replica:0/task:0/device:CPU:0"
    server_device = "/job:localhost/replica:0/task:0/device:CPU:0"

    # construct private input of server
    with tf.device(server_device):
      server_elements = tf.constant(["a", "b", "c"])

    # construct private inputs of client
    with tf.device(client_device):
      client_elements = tf.constant(["a", "b", "c", "d"])
      client_values = tf.constant([10, 20, 40, 80])

    # use protocol to securely compute intersection size and sum
    protocol_instance = PrivateIntersectionSum(client_device, server_device)
    client_result_op, server_wait_op = protocol_instance(client_elements, client_values, server_elements)

    # print private result (which is local to the client)
    with tf.device(client_device):
      intersection_size, intersection_sum = client_result_op
      print_size_op = tf.print("Intersection size: ", intersection_size)
      print_sum_op = tf.print("Intersection sum: ", intersection_sum)
      print_op = tf.group(print_size_op, print_sum_op)

    # run in TensorFlow session
    with tf.Session() as sess:
      sess.run([print_op, server_wait_op])

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
