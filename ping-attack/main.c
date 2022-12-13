/*
@Email: zl2272001@gmail.com
@Date: 2022/12/6
@Author:lizhong
@Target:
*/

# include "ping.h"

int main(int argc, char **argv)
{
    WSADATA wsaData;
    SOCKET sockRaw;
    struct sockaddr_in dest, from;
    struct hostent *hp;
    int bread, datasize;
    int fromlen = sizeof(from);
    char *dest_ip;
    char *icmp_data;
    char *recvbuf;
    unsigned int addr = 0;
    unsigned short seq_no = 0;

    // 初始化网络环境
    if (WSAStartup(0x0101, &wsaData) != 0)  // 与2.2版本socket库绑定
    {
        fprintf(stderr, "WSAStartup failed: %d\n", GetLastError());
        ExitProcess(STATUS_FAILED);
    }
    /*
    if (argc < 2)
    {
        Usage(argv[0]);
        ExitProcess(STATUS_FAILED);
    }
    */
    if ((sockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) == INVALID_SOCKET)
    {
        fprintf(stderr, "WSAStartup failed: %d\n", GetLastError());
        ExitProcess(STATUS_FAILED);
    }

    memset(&dest, 0, sizeof(dest));
    // hp = gethostbyname(argv[1]);
    hp = gethostbyname("www.baidu.com");   //39.156.66.10

    if (hp != NULL)
    {
        memcpy(&(dest.sin_addr), hp->h_addr, hp->h_length);
        dest.sin_family = AF_INET;
        dest_ip = inet_ntoa(dest.sin_addr);
    }
    else
    {
        //fprintf(stderr, "Unable to resolve %s\n", argv[1]);
        fprintf(stderr, "Unable to resolve %s\n", "www.baidu.com");
        ExitProcess(STATUS_FAILED);
    }

    datasize = sizeof(IcmpHeader);   // 12
    icmp_data = xmalloc(MAX_PACKET); // 分配1024 bytes 堆 给ICMP报文
    recvbuf = xmalloc(MAX_PACKET);   // 分配1024 bytes 堆

    if (!icmp_data)
    {
        fprintf(stderr, "HeapAlloc failed %d\n", GetLastError());
        ExitProcess(STATUS_FAILED);
    }

    memset(icmp_data, 0, MAX_PACKET);
    fill_icmp_head(icmp_data);

    printf("开始循环：\n");
    while (1)
    {
        int bwrote;
        ((IcmpHeader *)icmp_data)->i_cksum = 0;
        ((IcmpHeader *)icmp_data)->timestamp = GetTickCount();
        ((IcmpHeader *)icmp_data)->i_seq = seq_no++;
        ((IcmpHeader *)icmp_data)->i_cksum = checksum((USHORT *)icmp_data, sizeof(IcmpHeader));

        bwrote = sendto(sockRaw, icmp_data, datasize, 0, (struct sockaddr *)&dest, sizeof(dest));

        if (bwrote == SOCKET_ERROR)
        {
            fprintf(stderr, "sendto failed: %d\n", WSAGetLastError());
            ExitProcess(STATUS_FAILED);
        }
        if (bwrote < datasize)
        {
            fprintf(stdout, "Wrote %d bytes\n", bwrote);
        }

        bread = recvfrom(sockRaw, recvbuf, MAX_PACKET, 0, (struct sockaddr *)&from, &fromlen);

        if (bread == SOCKET_ERROR)
        {
            if (WSAGetLastError() == WSAETIMEDOUT)
            {
                printf("timed out\n");
                continue;
            }
            fprintf(stderr, "recvfrom failed: %d\n", WSAGetLastError());

            perror("revffrom failed.");
            ExitProcess(STATUS_FAILED);
        }
        decode_resp(recvbuf, bread, &from);
        //sleep(1);
    }


    closesocket(sockRaw);
    xfree(icmp_data);
    xfree(recvbuf);

    // 清理网络环境，释放Socket所占的资源
    WSACleanup(); /* clean up ws2_32.dll */
    system("pause");

    return 0;
}
