
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
    // 1.创建套接字
    int sock;
    struct sockaddr_in serv_addr;

    if((sock = socket(PF_INET, SOCK_DGRAM, 0))<0){
        err_log("fail socket.");
    }
    printf("客户端创建套接字成功.\n");
    sleep(3);

    // 2.服务器地址信息
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);

    // 3.不断获取用户输入并发送给服务器，然后接受服务器数据
    struct sockaddr from_addr;
    socklen_t from_addr_size = sizeof(from_addr);
    while(1){
        char buffer[BUF_SIZE] = {0};
        printf("Input a string:");
        gets(buffer);
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        int str_len = recvfrom(sock, buffer, BUF_SIZE, 0, &from_addr, &from_addr_size);
        buffer[str_len] = 0;

        printf("Message form server: %s\n", buffer);
        sleep(3);
    }

    close(sock);
    return 0;
}
