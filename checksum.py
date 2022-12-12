#!/usr/bin/python3
#-*-coding:utf-8-*-
"""
@Email: zl2272001@gmail.com
@Date: 2022/12/6
@Author:lizhong
@Target: Check Summing
"""

def chesksum(data):
	n = len(data)
	m = n % 2 #判断data长度是否是偶数字节
	sum = 0 #记录(十进制)相加的结果
	for i in range(0, n - m ,2): #将每两个字节(16位)相加（二进制求和）直到最后得出结果
		sum += ord(data[i]) + (ord(data[i+1]) << 8)#传入data以每两个字节（十六进制）通过ord转十进制，第一字节在低位，第二个字节在高位
	if m: #传入的data长度是奇数，将执行，且把这个字节（8位）加到前面的结果
		sum += ord(data[-1])
	#将高于16位与低16位相加
	sum = (sum >> 16) + (sum & 0xffff)
	sum += (sum >> 16) #如果还有高于16位，将继续与低16位相加
	answer = ~sum & 0xffff#对sum取反(返回的是十进制)
	#主机字节序转网络字节序列（参考小端序转大端序）
	answer = answer >> 8 | (answer << 8 & 0xff00)
	return answer #最终返回的结果就是wireshark里面看到的checksum校验和


def ICMP():
    data_type = '\x08' # ICMP Echo Request
    data_code = '\x00' # must be zero
    data_checksum = '\x57\xee' # "...with value 0 substituted for this field..."
    data_ID = '\x5c\x78' #Identifier
    data_Sequece = '\x00\x00' #Sequence number
    timestamp_from_icmp_data = "\x63\x75\xbc\x1d\x00\x0d\x38\xf6" 
    payload_body = '\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37' #data
    icmp_message = data_type + data_code + data_checksum + data_ID + data_Sequece + timestamp_from_icmp_data + payload_body
    print(icmp_message)
    return icmp_message

def IPv4():
    # TODO
    ver_hea = '\x45'
    ser_type = '\x00'
    total_length = '\x00\x54'
    id = '\x8d\xc0'
    flags_off = '\x00\x00'
    ttl = '\x40'
    protocal = '\x01'
    checksum = '\xa4\xd1'  # \xa4\xd1
    src = '\xac\x16\x32\x53'
    dst = '\x27\x9c\x42\x12'

    ip_message = ver_hea + ser_type + total_length + id + flags_off +\
                ttl + protocal + checksum + src + dst 
    return ip_message


if __name__ == "__main__":
    # message = ICMP()
    message = IPv4()
    int_ = chesksum(message)
    print('{:d} --->  {:x}'.format(int_,int_))
