/*
@Email: zl2272001@gmail.com
@Date: 2022/12/10
@Author:lizhong
@Target:
*/


/* main.c */
#include "traceroute.h"

struct troptions troptions;
uint16_t dport = 32768 + 666;

void usage(void)
{
    fprintf(stderr,
            "Usage traceroute"
            " [-"
            "muwIUT"
            "]" USAGE_NEWLINE
            " [-m tt]"
            " [-q nqueries]"
            " [-w waittime]"
            " destination"
            "\n");
}

int main(int argc, char *argv[])
{
    char c;
    struct hostent *hostent;
    in_addr_t saddr;

    /* 设置默认选项 */
    troptions.tro_maxttl = DFLOPT_MAXTTL;
    troptions.tro_nqueries = DFLOPT_NQUERIES;
    troptions.tro_waittime = DFLOPT_WAITTIME;
    troptions.tro_type = DFLOPT_TYPE;

    /* 处理用户输入 */
    printf("处理用户输入：/n");
    while ((c = getopt(argc, argv, "m:q:w:ITU")) != -1)
    {
        switch (c)
        {
        case 'm':
            if ((troptions.tro_maxttl = atoi(optarg)) <= 1)
            {
                fprintf(stderr, "invvalid -m value \n");
                exit(EXIT_ERR);
            }
            break;

        case 'q':
            if ((troptions.tro_nqueries = atoi(optarg)) < 1)
            {
                fprintf(stderr, "invalid -q value \n");
                exit(EXIT_ERR);
            }
            break;

        case 'w':
            if ((troptions.tro_waittime = atoi(optarg)) < 1)
            {
                fprintf(stderr, "invalid -w value \n");
                exit(EXIT_ERR);
            }
            break;

        case 'I':
            troptions.tro_type = TRACE_TYPE_ICMP;
            break;

        case 'T':
            troptions.tro_type = TRACE_TYPE_TCP;
            break;

        case 'U':
            troptions.tro_type = TRACE_TYPE_UDP;
            break;

        case '?':
            fprintf(stderr, "unrecognized option: %c ", c);
            exit(EXIT_ERR);
        }
    }

    if (optind != argc - 1)
    {
        usage();
        exit(EXIT_ERR);
    }
    hostname = argv[optind];

    /* 处理用户输入，并构建一个目的端套接字地址结构 */
    if ((saddr = inet_addr(hostname)) == INADDR_NONE)
    {
        if ((hostent = gethostbyname(hostname)) == NULL)
        {
            fprintf(stderr, "unknow host %s \n", hostname);
            exit(EXIT_ERR);
        }
        memmove(&saddr, hostent->h_addr, hostent->h_length);
    }
    bzero(&destaddr, sizeof(destaddr)); // 字符串清零
    destaddr.sin_family = AF_INET;
    destaddr.sin_addr.s_addr = saddr;

    /* 调用do_trace函数，由其决定具体要调用的探测方法 */
    do_trace();

    return 0;
}
