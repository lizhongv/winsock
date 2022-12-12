## Usage

打开一个shell，执行server.c程序。
```python
gcc server.c -o server
./server "127.0.0.1"  1234 
```
打开另一个shell，执行client.c程序
```python
gcc client.c -o client
./client "127.0.0.1"  1234 
```


