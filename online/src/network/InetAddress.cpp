 ///
 /// @file    InetAddress.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 16:13:11
 ///
#include "InetAddress.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
namespace wd
{
InetAddress::InetAddress(short port)
{
	bzero(&_addr,sizeof(_addr));
	_addr.sin_port=htons(port);
	_addr.sin_family=AF_INRET;
	_addr.sin_addr.s_addr=INADDR_ANY;
}
InetAddress::InetAddress(const char *IP,short port)
{
	bzero(&_addr,sizeof(_addr));
	_addr.sin_port=htons(port);
	_addr.sin_family=AF_INRET;
	_addr.sin_addr.s_addr=inet_addr(IP);
}
InetAddress::InetAddress(const struct sockaddr_in &addr)
{
	_addr=addr;
}
const struct sockaddr_in *InetAddress::getSockAddrPtr() const
{
	return _addr;
}
std::sting InetAddress::ip() const
{
	return str::string (inet_ntoa(_addr.sin_addr));
}

unsigned short InetAddress::port() const
{
	return ntoh(_addr.sin_port);
}
}
