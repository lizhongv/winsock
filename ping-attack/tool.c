/*
@Email: zl2272001@gmail.com
@Date: 2022/12/6
@Author:lizhong
@Target:
*/

#include "ping.h"

USHORT checksum(USHORT *buffer, int size)
{
    unsigned long cksum = 0;
    while (size > 1)
    {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }
    if (size)
    {
        cksum += *(UCHAR *)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (USHORT)(~cksum);
}

/* The response is an IP packet. We must decode */
/* the IP header to locate the ICMP data */
void decode_resp(char *buf, int bytes, struct sockaddr_in *from)
{
    IpHeader *iphdr;
    IcmpHeader *icmphdr;
    unsigned short iphdrlen;

    iphdr = (IpHeader *)buf;
    iphdrlen = iphdr->h_len * 4; // number of 32-bit words *4 = bytes
    if (bytes < iphdrlen + ICMP_MIN)
    {
        printf("Too few bytes from %s\n", inet_ntoa(from->sin_addr));
    }

    icmphdr = (IcmpHeader *)(buf + iphdrlen);
    if (icmphdr->i_type != ICMP_ECHOREPLY)
    {
        fprintf(stderr, "non-echo type %d recvd\n", icmphdr->i_type);
        return;
    }
    if (icmphdr->i_id != (USHORT)GetCurrentProcessId())
    {
        fprintf(stderr, "someone else's packet!\n");
        return;
    }
    printf("解析: ");
    printf("%d bytes from %s:", bytes, inet_ntoa(from->sin_addr));
    printf(" icmp_seq = %d. ", icmphdr->i_seq);
    printf(" time: %d ms ", GetTickCount() - icmphdr->timestamp);
    printf("\n");
}

void fill_icmp_head(char *icmp_data)
{
    IcmpHeader *icmp_hdr;
    icmp_hdr = (IcmpHeader *)icmp_data;
    icmp_hdr->i_type = ICMP_ECHO;
    icmp_hdr->i_code = 0;
    icmp_hdr->i_cksum = 0;
    icmp_hdr->i_id = (USHORT)GetCurrentProcessId();
    icmp_hdr->i_seq = 0;
}

void Usage(char *progname)
{
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "%s <host>\n", progname);
    ExitProcess(STATUS_FAILED);
}
