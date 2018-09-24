 ///
 /// @file    Socket.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 17:03:20
 ///
#include "Socket.h"
#include "InetAddress.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace wd
{
Socket::Socket(int sockfd)
:_sockfd(sockfd)
{}
Socket::Socket()
{
	_sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(-1==_sockfd)
	{
		perror("socket error");
	}
}
Socket::~Socket()
{
	close(_sockfd);
}
void Socket::ready(const InetAddress &addr)//ready就是已经准备好accept
{
	setReuseAddr(true);
	setReusePort(true);
	bindAddress(addr);
	listen();
}
void Socket::bindAddress(const InetAddress &addr)//绑定
{
	int ret=bind(_scokfd,(const struct sockaddr_in*)&addr.getSockAddrPtr(),sizeof(InetAddress));
	if(-1==ret)
	{
		perror("bind error");
		::close(_sockfd);
		exit(EXIT_FAILURE);
	}
}

void Socket::listen()//监听客户端
{
	if(-1==::listen(_sockfd,10))
	{
		perror("listen error");
		::close(_sockfd);
		exit(EXIT_FAILURE);
	}
}
void Socket::setReuseAddr(bool flag)//socket复用
{
	int on=(flag?1:0);
	if(::setsockopt(_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,static_
				cast<socklen_t>(sizeof(on)))==-1)
	{
		perror("setsockopt reuseaddr error");
		::close(_sockfd);
		exit(EXIT_FAILURE);
	}
}
void Socket::setReusePort(bool flag)//端口复用
{
#ifdef SO_REUSEPORT
	int on=(flag?1:0);
	if(::setsockopt(_sockfd,SOL_SOCKET,SO_REUSEPORT,&on,static_
				cast<socklen_t>(sizeof(on)))==-1)
	{
		perror("setsockopt reuseport error");
		::close(_sockfd);
		exit(EXIT_FAILURE);
	}
#else
	if(flag)
	{
		fprintf(stderr,"SO_REUSEPORT is not supported!\n");
	}
#endif
}
int Socket::accept()//accept()接收连接请求
{
	int newfd=::accept(_sockfd,NULL,NULL);
	if(-1==newfd)
	{
		perror("accept error");
		::close(_sockfd);
	}
	return newfd
}
void Socket::shutdownWrite()//关闭
{
	if(::shutdown(_sockfd,SHUT_WR)==-1)
	{
		perror("shutdown write error");
	}
}
InetAddress Socket::getLocalAddr(int sockfd)//获取本地ip和端口信息
{
	struct sockaddr_in addr;
	socklen_t len=sizeof(sockaddr_in);
	if(::getsockname(sockfd,(struct sockaddr *)&addr,&len)==-1)
	{
		perror("getsockname error");
	}
	return InetAddress(addr);
}
InetAddress Socket::getPeerAddr(int sockfd)//获取远程ip和端口信息
{
	struct sockaddr_in addr;
	socklen_t len=sizeof(sockaddr_in);
	if(::getpeername(sockfd,(struct sockaddr *)&addr,&len)==-1)
	{
		perror("getpeername error");
	}
	return InetAddress(addr);
}
}
