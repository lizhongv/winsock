#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 100
#define RES_SIZE 50
#define  err_log(errlog) do{ perror(errlog); exit(1); }while(0)

int main(){
    int serv_sock;
    struct sockaddr_in serv_addr, clnt_addr;

    // 1.创建套接字
    if((serv_sock = socket(PF_INET, SOCK_DGRAM, 0))<0){
        err_log("fail to sock.");
    }
    printf("服务器创建套接字成功！\n");
    sleep(3);

    
    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //自动获取IP地址
    serv_addr.sin_port = htons(1234);  //端口

    // 2. 绑定套接字
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        err_log("fail to bing.");
    }
    printf("服务器绑定套接字成功.\n");
    sleep(3);


    socklen_t clnt_addr_size = sizeof(clnt_addr);
    char buffer[BUF_SIZE];  //缓冲区
    while(true){
        // 3. 接收客户端请求
        int str_len = recvfrom(serv_sock, buffer, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        printf("从客户端接收消息：%s\n", buffer);
        sleep(3);

        // 4. 向客户端发送消息
        char response[RES_SIZE] = "OK，信息已收到。";
        sendto(serv_sock, response, RES_SIZE, 0, (struct sockaddr*)&clnt_addr, clnt_addr_size);
        printf("向客户端发送消息：%s\n", response);
    }

    close(serv_sock);

    return 0;
}
