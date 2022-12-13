## Usage

命令行参数可选项
```shell
[optiaon][-m][-q][-w][IUT] destination
````

基于ICMP的traceroute：
```shell
gcc main.c traceroute.c -o  trace
sudo ./trace -I www.baidu.com
```

基于UDP的traceroute
```shell
gcc main.c traceroute.c -o  trace
sudo ./trace -U www.baidu.com
```
