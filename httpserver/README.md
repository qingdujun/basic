# c++11 via boost httpserver

## How to run it

运行时，参数一次为`IP`、`PORT`以及服务器根路径。

```bash
$ cmake .
$ make
$ ./http_server 0.0.0.0 9527 ~/home/www/
```

此时，可以在浏览器中直接访问：
```
http://localhost:9527/

or 

http://localhost:9527/abc.txt

or 

http://localhost:9527/s?key=ml&val=hua7th
```