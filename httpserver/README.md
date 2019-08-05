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


# 压力测试

模拟300个用户，访问60秒

## HTTP Proxy压力测试
使用Webbench做压力测试结果，

1471次请求失败
```
MacBookPro:webbench-1.5 qingdujun$ webbench -c 300 -t 60 http://localhost:9527/
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://localhost:9527/
300 clients, running 60 sec.

Speed=34237 pages/min, 44780 bytes/sec.
Requests: 32766 susceed, 1471 failed.
```

## 百度压力测试

对百度首页压力测试结果，

0次请求失败
```
MacBookPro:webbench-1.5 qingdujun$ webbench -c 300 -t 60 http://www.baidu.com/
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://www.baidu.com/
300 clients, running 60 sec.

Speed=4025 pages/min, 10963105 bytes/sec.
Requests: 4025 susceed, 0 failed.
```

貌似webbench不能模拟更多的用户，fork failed.....
```
MacBookPro:webbench-1.5 qingdujun$ webbench -c 500 -t 10 http://www.baidu.com/
Webbench - Simple Web Benchmark 1.5
Copyright (c) Radim Kolar 1997-2004, GPL Open Source Software.

Benchmarking: GET http://www.baidu.com/
500 clients, running 10 sec.
problems forking worker no. 464
fork failed.: Operation timed out
```
