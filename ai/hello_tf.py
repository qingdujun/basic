# -*- coding:utf-8 -*-
import tensorflow as tf

def hello_tf():
    tf.enable_eager_execution()
    print(tf.reduce_sum(tf.random_normal([1000, 1000])))

if __name__ == "__main__":
    hello_tf()