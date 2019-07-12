# conding=utf-8
# 
# 2019-05-31 21:51:27 by Manlou(hua7th@gmail.com)
#
import numpy as np

def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x):
    return sigmoid(x) / (1 - sigmoid(x))

class Neuron(object):
    def __init__(self, cell_shape):
        self.input = np.ones(cell_shape)
        self.weights = np.random.random(cell_shape) * 0.1
        self.deltas = 0.0
        self.bias = 0.0
        self.output = 1.0
        self.change = 0
    
    def calculate_output(self, x):
        self.input = x
        self.output = sigmoid(np.dot(self.weights, self.input))
        return self.output

    def get_back_weight(self):
        return np.dot(self.weights, self.deltas)

    def update_weight(self, target = 0 , back_weight = 0, learning_rate = 0.1, layer = 'OUTPUT'):
        if layer == 'OUTPUT':
            self.deltas = np.dot((target - self.output), sigmoid_derivative(self.input))
        elif layer == 'HIDDEN':
            self.deltas = np.dot(back_weight, sigmoid_derivative(self.input))
        
        delta = np.dot(self.input, self.deltas) * learning_rate + np.dot(self.change, 0.9)
        self.weights += delta
        self.change = delta

class NetLayer(object):
    def __init__(self, cell_shape, layer_shape):
        self.nerons = [Neuron(cell_shape) for _ in range(layer_shape)]
        self.next = None

    def calculate_output(self, x):
        output = np.array([cell.calculate_output(x) for cell in self.nerons])
        if self.next is not None:
            return self.next.calculate_output(output)
        return output

    def get_back_weight(self):
        return sum([cell.get_back_weight() for cell in self.nerons])

    def update_weight(self, learning_rate, target):
        layer = 'OUTPUT'
        back_weight = np.zeros(len(self.nerons))
        if self.next is not None:
            back_weight = self.next.update_weight(learning_rate, target)
            layer = 'HIDDEN'
        for i, cell in enumerate(self.nerons):
            item = 0 if len(target) <= i else target[i]
            cell.update_weight(target=item, back_weight=back_weight[i], learning_rate=learning_rate, layer=layer)
        return self.get_back_weight()

class NeuralNetwork(object):
    def __init__(self, layers):
        self.layers = []
        self.cnstruct_network(layers)

    def cnstruct_network(self, layers):
        last_layer = None
        for i, layer in enumerate(layers):
            if i == 0:
                continue
            cur_layer = NetLayer(layer, layers[i-1])
            self.layers.append(cur_layer)
            if last_layer is not None:
                last_layer.next = cur_layer
            last_layer = cur_layer
    
    def fit(self, x, y, learning_rate=0.1, epochs=10000, shuffle=False):
        indices = np.arange(len(x))
        for _ in range(epochs):
            if shuffle:
                np.random.shuffle(indices)
            for i in indices:
                self.layers[0].calculate_output(x[i])
                self.layers[0].update_weight(learning_rate, y[i])
    
    def predict(self, x):
        return self.layers[0].calculate_output(x)