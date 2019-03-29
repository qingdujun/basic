# -*- coding:utf-8 -*-
import jieba
import numpy as np 
import collections
import random
# import tensorflow as tf
# import numpy as np
# import random
# import collections
# from collections import Counter
# import jieba
# from sklearn.manifold import TSNE
# import matplotlib as mpl
# import matplotlib.pyplot as plt

def get_ch_lable(txt_file):  
    labels= ""
    print(txt_file)
    with open(txt_file, 'rb') as f:
        for label in f: 
            labels =labels+label.decode('utf-8')
    return labels

def get_split_word(training_data):
    seg_list = jieba.cut(training_data)
    training_ci = " ".join(seg_list)
    training_ci = training_ci.split()
    training_ci = np.array(training_ci)
    training_ci = np.reshape(training_ci, [-1, ])
    return training_ci

def build_dataset(words, top_n):
    count = [['UNK', -1]]
    count.extend(collections.Counter(words).most_common(top_n - 1))
    dictionary = dict()
    for word, _ in count:
        dictionary[word] = len(dictionary)
    data = list()
    unk_count = 0
    for word in words:
        if word in dictionary:
            index = dictionary[word]
        else:
            index = 0  # dictionary['UNK']
            unk_count += 1
        data.append(index)
    count[0][1] = unk_count
    reversed_dictionary = dict(zip(dictionary.values(), dictionary.keys()))
    return data, count, dictionary, reversed_dictionary

data_index = 0

def generate_batch(data,batch_size, num_skips, skip_window):
    global data_index
    assert batch_size % num_skips == 0
    assert num_skips <= 2 * skip_window

    batch = np.ndarray(shape=(batch_size), dtype=np.int32)
    labels = np.ndarray(shape=(batch_size, 1), dtype=np.int32)
    span = 2 * skip_window + 1  # [ skip_window target skip_window ]
    buffer = collections.deque(maxlen=span)

    if data_index + span > len(data):
        data_index = 0

    buffer.extend(data[data_index : data_index + span])
    data_index += span

    for i in range(batch_size // num_skips):
        target = skip_window  # target label at the center of the buffer
        targets_to_avoid = [skip_window]
        for j in range(num_skips):
            while target in targets_to_avoid:
                target = random.randint(0, span - 1)
            targets_to_avoid.append(target)
            batch[i * num_skips + j] = buffer[skip_window]
            labels[i * num_skips + j, 0] = buffer[target]

        if data_index == len(data):
            #print(data_index,len(data),span,len(data[:span]))
            #buffer[:] = data[:span]
            buffer = data[:span]
            data_index = span
        else:
            buffer.append(data[data_index])
            data_index += 1

    # Backtrack a little bit to avoid skipping words in the end of a batch
    data_index = (data_index + len(data) - span) % len(data)
    return batch, labels

if __name__ == "__main__":
    training_data = get_ch_lable('test.txt')
    jieba_data = get_split_word(training_data)
    training_label, count, dictionary, words = build_dataset(jieba_data, 350)
    # print(training_label, count, dictionary, words)
    words_size = len(dictionary)
    # print('Sample data', training_label[:10], [words[i] for i in training_label[:10]])
    batch, labels = generate_batch(training_label,batch_size=8, num_skips=2, skip_window=1)
    print(batch)
    print('------')
    print(labels)

    
