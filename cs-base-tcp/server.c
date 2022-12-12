/*server.c*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define  err_log(errlog) do{ perror(errlog); exit(1); }while(0)

int main(int argc, const char *argv[]){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
      
    if(argc != 3) {  
        fprintf(stderr,"Usage:%s serverip port.\n", argv[0]);
        return -1;
    }

    // 1.创建套接字       
    if((serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0) {
        err_log("fail to socket.");
    }
    printf("服务器套接字创建成功！\n");
    sleep(3);
   

    memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
    serv_addr.sin_family = AF_INET;  //使用IPv4地址
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);  //服务器IP地址     
    serv_addr.sin_port = htons(atoi(argv[2]));  //服务器端口                         
    
    // 2. 绑定套接字       
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        err_log("fail to bind.");
    }
    printf("服务器套接字绑定成功！\n");
    sleep(3);


    // 3. 进入监听状态，等待用户发起请求      
    if(listen(serv_sock, 20)<0){
        err_log("fail to listen.");
    }
    printf("服务器监听客户端请求....\n");
    

    // 4. 接收客户端请求
    socklen_t clnt_addr_size = sizeof(clnt_addr);
    if((clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size))<0){
        err_log("fail to accept.");
    }
    printf("服务器和客户端已经建立连接.\n");
    sleep(3);

    // 5. 向客户端发送数据
    printf("向客户端发送数据.....\n");
    char str[] = "hello man, I am lizhong.";
    write(clnt_sock, str, sizeof(str));
    sleep(3);
   
    // 6.关闭套接字
    close(clnt_sock);
    close(serv_sock);
    printf("关闭服务器套接字.\n");

    return 0;
}
