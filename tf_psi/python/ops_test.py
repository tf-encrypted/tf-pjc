import numpy as np
import tensorflow as tf
from tensorflow.python.platform import test

from tf_psi.python.ops import psi_launch_server

class PsiTest(test.TestCase):

  def test_launch_server(self):
    elements = tf.constant([1, 2, 3, 4], dtype=tf.int32)
    launch_op = psi_launch_server(elements)

    with tf.Session() as sess:
      sess.run(launch_op)
      assert True

if __name__ == '__main__':
  test.main()
