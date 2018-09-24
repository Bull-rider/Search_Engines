///
/// @file    SocketIO.cpp
/// @author  lemon(haohb13@gmail.com)
/// @date    2018-09-23 21:21:29
///
#include "SocketIO.h"
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace wd
{
SocketIO::SocketIO(int sockfd)
	:_sockfd(sockfd)
{}
size_t SocketIO::readn(char *buf,size_t count)
{
	size_t total=0;
	size_t ret;
	while(total<count)
	{
		ret=read(_sockfd,buf+total,count-total);
		if(-1==ret)
		{
			if(errno==EINTR)//胡咯中断信号
				continue;
			return EXIT_FAILURE;
		}
		else if(ret==0)
		{
			break;
		}
		total+=ret;
	}
	return 0;
}
size_t SocketIO::writen(const char *buf,size_t count)
{
	size_t total=0;
	size_t ret;
	while(total<count)
	{
		ret=write(_sockfd,buf+total,count-total);
		if(-1==ret)
		{
			if(errno==EINTR)
				continue;
			return EXIT_FAILURE;
		}
		total+=ret;
	}
	return 0;
}
siae_t SockIO::recv_peek(char *buf,size_t count)
{
	int nread;
	do
	{
		nread=::recv(_sockfd,buf,count,MSG_PEEK);
	}while(nread==-1&&errno==EINTR);
	return nread;
}

size_t SocketIO::readline(char *buf,size_t maxlen)
{
	size_t nleft=maxlen-1;
	char *pbuf=buf;
	size_t total=0;
	while(nleft>0)
	{
		int nread=recv_peek(pbuf,nleft);
		if(nread<=0)
			return nread;
		for(size_t idx=0;idx!=nread;++idx)
		{
			if(pbuf[idx]=='\n')
			{
				size_t nsize=idx+1;
				if(readn(pbuf,nsize)!=nsize)
					return EXIT_FAILURE;
				pbuf+=nsize;
				total+=nsize;
				*pbuf=0;
				return total;
			}
		}
		if(readn(pbuf,nread)!=nread)
			return EXIT_FAILURE;
		pbuf+=nread;
		nleft-=nread;
		total+=nread;
	}
	*pbuf=0;
	return maxlen-1;
}
}
