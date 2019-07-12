README.md


# numpy.reshape
`-1` will auto calculate value....
```python
>>> import numpy as np
>>> x = np.array([1,2,3,4,5,6])
>>> x
array([1, 2, 3, 4, 5, 6])
>>> X = x.reshape(2,-1)
>>> X
array([[1, 2, 3],
       [4, 5, 6]])
>>> x.shape[0]
6
```

# numpy.newaxis
newaxis is a alias of `None`. In another words, `np.newaxis = None`.
```python
>>> x = np.arange(3)
>>> x
array([0, 1, 2])
>>> x.shape
(3,)
>>> x[:,np.newaxis]
array([[0],
       [1],
       [2]])
```
another example
```python
>>> b = np.arange(6)
>>> b
array([0, 1, 2, 3, 4, 5])
>>> b[None, ...]
array([[0, 1, 2, 3, 4, 5]])
```

# x.T
It's a matrix transpose.
```python
>>> X
array([[1, 2, 3],
       [4, 5, 6]])
>>> X.T
array([[1, 4],
       [2, 5],
       [3, 6]])
```

# np.maximum
calculate ReLU
```python
>>> x = np.array([1,2,-5,0,7,-8,3])
>>> x
array([ 1,  2, -5,  0,  7, -8,  3])
>>> np.maximum(x,0)
array([1, 2, 0, 0, 7, 0, 3])
```

# np.random.randn
Return a sample (or samples) from the "standard normal" distribution.
```python
>>> np.random.randn(2,3)
array([[-0.69891595, -0.01521035,  0.35399943],
       [-0.65313309, -2.45569047, -0.13710124]])
>>> w = np.random.randn(2,3)
>>> w
array([[ 0.07617784, -0.73325498,  0.7902037 ],
       [-1.04575581,  0.37697524,  0.91252618]])
>>> np.mat(w)
matrix([[ 0.07617784, -0.73325498,  0.7902037 ],
        [-1.04575581,  0.37697524,  0.91252618]])
```

# y = WX + b and y = XW + b?
WX = X.T.dot(W.T)

