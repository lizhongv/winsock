#!/usr/bin/python3
#-*-coding:utf-8-*-
"""
@Email: zl2272001@gmail.com
@Date: 2022/12/6
@Author:lizhong
@Target: the client program is based on udp.
"""
    
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define  err_log(errlog) do{ perror(errlog); exit(1); }while(0)
#define BUF_SIZE 100

int main(){
    int clnt_sock;
    struct sockaddr_in serv_addr;

    // 1.创建套接字
    if((clnt_sock = socket(PF_INET, SOCK_DGRAM, 0))<0){
        err_log("fail socket.");
    }
    printf("客户端套接字创建成功.\n");
    sleep(3);

    // 2.服务器地址信息
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);

    struct sockaddr from_addr;
    socklen_t from_addr_size = sizeof(from_addr);
    while(1){
        char buffer[BUF_SIZE] = {0};
        printf("Input Message: ");
        gets(buffer);
       
        // 3.不断获取用户输入，并发送给服务器，然后接受服务器数据
        sendto(clnt_sock, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        // 4. 接收服务器的回复信息
        int str_len = recvfrom(clnt_sock, buffer, BUF_SIZE, 0, &from_addr, &from_addr_size);
        buffer[str_len] = 0;
        printf("Message form server: %s\n", buffer);
        sleep(3);
    }

    // 4. 关闭套接字
    close(clnt_sock);
    return 0;
}
