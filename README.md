# TF PJC

TF PJC provides a bridge between TensorFlow and Google's [Private Join and Compute](https://github.com/google/private-join-and-compute) library. This allows two parties to privately compute the intersection of two sets and the sum of associated values as described in [IKNP+'19](https://eprint.iacr.org/2019/723).

<!--
[![PyPI](https://img.shields.io/pypi/v/tf-pjc.svg)](https://pypi.org/project/tf-pjc/) [![CircleCI Badge](https://circleci.com/gh/tf-encrypted/tf-pjc/tree/master.svg?style=svg)](https://circleci.com/gh/tf-encrypted/tf-pjc/tree/master)
-->

## Usage

The library may be used as shown in the following example:

```python
import tensorflow as tf
import tf_pjc

# device strings of the two players involved
client_device = "/job:localhost/task:0/device:CPU:0"
server_device = "/job:localhost/task:1/device:CPU:0"

# construct private input of server
with tf.device(server_device):
  server_elements = tf.constant(["a", "b", "c"])

# construct private inputs of client
with tf.device(client_device):
  client_elements = tf.constant(["a", "b", "c", "d"])
  client_values   = tf.constant([100, 200, 400, 800])

# use protocol to securely compute intersection size and sum
protocol_instance = tf_pjc.PrivateIntersectionSum(client_device, server_device)
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
```

Future releases will also include the possibility of using TF PJC in conjunction with [TF Encrypted](https://github.com/tf-encrypted/tf-encrypted) as a kernel for `tfe.sets.intersection_sum`.

## Installation

Python 3 packages are available from [PyPI](https://pypi.org/project/tf-pjc/):

```
pip install tf-pjc
```

