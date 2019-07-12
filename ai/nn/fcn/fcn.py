import numpy as np

def affine_forward(W, X, B):
    Z = np.dot(W.T, X) + B
    return Z

def sigmoid_forward(Z):
    A = 1 / (1 + np.exp(-Z))
    return A

def softmax_loss(a, y):
    loss = -np.mean(np.sum(np.dot(y, np.log(a)) + np.dot(1-y, np.log(1-a))))
    return loss

def gradient_descent(dz):
    dw = np.mean(np.dot(x, dz.T))
    db = np.mean(dz)
    return dw, db

def affine_backward(z, c):
    x, _, _ = c
    X = x.reshape(x.shape[0], -1)
    dw = np.dot(X, z.T)
    db = np.dot(np.ones(x.shape[0]), z.T)
    return dw, db

def softmax(x):
    exp = np.exp(x - x.max())
    return exp/exp.sum()

def sigmoid_backward(z):
    x = np.multiply(sigmoid_forward(z), 1-sigmoid_forward(z)) # NOT np.dot
    return x


class dnn(object):
    def __init__(self):
        pass

    def forward_propagation(self):
        pass

    def backward_propagation(self):
        pass
    

if __name__ == "__main__":
    ''' generate xor data '''
    x = np.mat([
        [0,0], [0,1], [1,0], [1,1]
    ])
    y = np.mat([0,1,1,0])
    ''' initialzation weights '''
    layers = [2,2,1] # three layers
    weights = []
    for i in range(len(layers)-1):
        w = np.random.randn(layers[i+1], layers[i]+1)
        weights.append(np.mat(w))
    print(weights)
    # [matrix([[-0.39343047, -2.2701338 ,  1.36745905],
    #     [-2.10672572,  0.48727075,  0.06383682]]), matrix([[-0.26778487, -0.6134959 , -1.00516374]])]
