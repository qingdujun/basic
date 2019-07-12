import numpy as np 

def sigmoid(Z):
    A = 1 / (1 + np.exp(-Z))
    return A

def sigmoid_backprogation(x):
    return np.multiply(x, 1-x)

def softmax(a, y):
    loss = np.dot(-y, np.log(a)) + np.dot(1-y, np.log(1-a))
    return loss

def gradient_descent(x):
    dw = 
    db = 
    return dw, db

if __name__ == "__main__":
    X = np.array([
        [0,0],
        [1,0],
        [0,1],
        [1,1]])
    Y = np.array([
        [0,1,1,0]
        ]).T

    w0 = np.random.randn(2,3)
    b0 = np.random.randn(2,1)

    w1 = np.random.randn(3,2)
    b1 = np.random.randn(3,1)

    w2 = np.random.randn(2,1)
    b2 = np.random.randn(2,1)

    for epoch in range(60000):
        hidden1 = sigmoid(np.dot(X, w0))
        hidden2 = sigmoid(np.dot(hidden1, w1))
        out = sigmoid(np.dot(hidden2, w2)) # softmax

        out_loss = Y - out

        if epoch % 5000 == 0:
            print("loss: "+str(out_loss))
        
        out_delta = np.multiply(out_loss, sigmoid_backprogation(out))
        
        hidden2_loss = np.dot(out_delta, w2.T)
        hidden2_delta = np.multiply(hidden2_loss, sigmoid_backprogation(hidden2))

        hidden1_loss = np.dot(hidden2_delta, w1.T)
        hidden1_delta = np.multiply(hidden1_loss, sigmoid_backprogation(hidden1))

        w2 += np.dot(hidden2.T, out_delta)
        w1 += np.dot(hidden1.T, hidden2_delta)
        w0 += np.dot(X.T, hidden1_delta)


    

