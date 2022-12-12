#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define  err_log(errlog) do{ perror(errlog); exit(1);}while(0)

int main(int argc, const char *argv[]){
    // 1.创建套接字
    int clnt_sock;
    struct sockaddr_in serv_addr;


    if((clnt_sock= socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0){
        err_log("fail to sock.");
    }
    printf("套接字创建成功！\n");
    sleep(5);

    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  //服务器IP地址
    serv_addr.sin_port = htons(atoi(argv[2]));  //服务器端口
    
    // 2.向服务器发起连接请求
    printf("发送连接请求...\n");
    if((connect(clnt_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0){
        err_log("fail to connect.");
    }
    printf("客户端和服务器已经建立连接.\n");
    sleep(5);
   
    // 3. 读取服务器回复信息
    char buffer[40];
    read(clnt_sock, buffer, sizeof(buffer)-1);
    printf("读取服务器返回数据.\n");
    sleep(5);
    printf("Message form server: %s\n", buffer);
    sleep(5);
   
    // 4.关闭套接字
    printf("关闭客户端套接字.\n");
    close(clnt_sock);

    return 0;
}
