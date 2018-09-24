 ///
 /// @file    TcpConnection.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 20:34:31
 ///
#include "TcpConnection.h"
#include "EpollPoller.h"
#include <string.h>
#include <stdio.h>
#include <iostream>

namespace wd
{
TcpConnection::TcpConnection(int sockfd,EpollPoller *loop)
:_sockfd(sockfd)
,_sockIO(sockfd)
,_localAddr(wd::Socket::getLocalAddr(sockfd))
,_peerAddr(wd::Socket::getPeerAddr(sockfd))
,_isShutdownWrite(false)
,_loop(loop)
{}
TcpConnection::~TcpConnection()
{
	if(!_isShutdownWrite)
	{
		_isShutdownWrite=true;
		shutdown();
	}
	std::cout<<"~TcpConnection()"<<std::endl;
}
std::string TcpConnection::receive()//接收数据
{
	char buf[1024];
	bzero(buf,sizeof(buf));
	size_t ret=_sockIO.readline(buf,sizeof(buf));//调用IO类中的函数
	if(ret==0)
	{
		return std::string();
	}
	else
		return std::string(buf);
}
void TcpConnection::send(const std::string &msg)//发送数据
{
	_sockIO.write(msg.c_str(),msg.size());
}
//针对网页请求，发送完数据就关闭
void TcpConnection::sendAndClose(const std::string &msg)
{
	send(msg);
	shutdown();
}
//这里将任务绑定到IO线程(工作线程)，执行send()函数
void TcpConnection::sendInLoop(const std::string &msg)
{
	_loop->runInLoop(std::bind(&TcpConnection::send,this,msg));
}
//关闭连接
void TcpConnection::shutdown()
{
	if(!_isShutdownWrite)
		_sockfd.shutdownWrite();
	_isShutdownWrite=true;
}
//返回连接的ip和端口信息
std::string TcpConnection::toString()
{
	char str[100];
	snprintf(str,sizeof(str),"%s:%d->%s:%d",
			_localAddr.ip().c_str(),
			_localAddr.port(),
			_peerAddr.ip().c_str(),
			_peerAddr.port());
	return std::string(str);
}
//设置相应的回调函数
void TcpConnection::setConnectionCallback(TcpConnectionCallback cb)
{
	_onConnectionCb=cb;
}
void TcpConnection::setMessageCallback(TcpConnectionCallback cb)
{
	_onMessageCb=cb;
}
void TcpConnection::setCloseCallback(TcpConnectionCallback cb)
{
	_onClosedCb=cb;
}
//调用相应的回调函数
void TcpConnection::handleConnectionCallback()
{
	if(_onConnectionCb)
		_onConnection(shared_from_this());
}
void TcpConnection::handleMessageCallback()
{
	if(_onMessageCb)
		_onMessageCb(shared_from_this());
}
void TcpConnection::handleCloseCallback()
{
	if(_onCloseCb)
		_onCloseCb(shared_from_this());
}
}
