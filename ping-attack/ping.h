/*
@Email: zl2272001@gmail.com
@Date: 2022/12/6
@Author:lizhong
@Target: the client program is based on tcp.
*/
    
#ifndef PING_H_INCLUDED
#define PING_H_INCLUDED


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <winsock2.h>
#include <windows.h>

#pragma comment (lib, "ws2_32.lib")

#define WIN32_LEAN_AND_MEAN  // 指示编译器不要包括与MFC相关的操作
#define ICMP_ECHO 8      // 请求回显ping请求 请求回显8
#define ICMP_ECHOREPLY 0 // 回显应答0
#define ICMP_MIN 12      // minimum 12 byte icmp message(just header)

#define STATUS_FAILED 0xFFFF
#define MAX_PACKET 1024
/*
堆分配
LPVOID HeapAlloc(HANDLE hHeap,DWORD dwFlags,SIZE_T dwBytes,);
*/
#define xmalloc(s) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, (s))
#define xfree(p) HeapFree(GetProcessHeap(), 0, (p))


/* The IP header 20 bytes*/
typedef struct iphdr
{
    unsigned char h_len : 4;       // length of the header
    unsigned char version : 4;     // Version of IP
    unsigned char tos;             // Type of service
    unsigned short total_len;      // total length of the packet
    unsigned short ident;          // unique identifier
    unsigned short frag_and_flags; // flags
    unsigned char ttl;
    unsigned char proto;     // protocol(TCP, UDP etc)
    unsigned short checksum; // IP checksum
    unsigned int sourceIP;
    unsigned int destIP;
} IpHeader;


/* ICMP header 12 bytes */
typedef struct _ihdr
{
    unsigned char i_type;
    unsigned char i_code; /* type sub code */
    unsigned short i_cksum;
    unsigned short i_id;
    unsigned short i_seq;
    /* This is not the std header, but we reserve space for time */
    unsigned long timestamp;
} IcmpHeader;

unsigned short checksum(unsigned short *, int);
void fill_icmp_head(char *);
void decode_resp(char *, int, struct sockaddr_in *);
void Usage(char *progname);

#endif // PING_H_INCLUDED
