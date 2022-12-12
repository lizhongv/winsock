## Usage

打开一个shell, 编译server.c程序，生成server可执行文件：
```python
  gcc server.c -o server
  ./server
```
打开另一个shell，编译client.c程序，生成client可执行文件:
```python
  gcc client.c -o client
  ./client
```
mac osx 查看端口占用情况：
```shell
lsof -i udp:1234
```
强制杀死进程
```shell
kill -9 69094
```
  
