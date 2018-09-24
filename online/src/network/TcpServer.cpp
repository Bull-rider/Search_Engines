 ///
 /// @file    TcpServer.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 14:58:23
 ///
#include "TcpServer.h"
namespace wd
{
TcpServer::TcpServer(unsigned short port)
:_inetAddr(port)
,_sockfd()
,_epollfd(_sockfd.fd())
{
	_sockfd.ready(_inetAddr);
}
TcpServer::TcpServer(const char *IP,unsigned short port)
:_inetAddr(IP,port)
,_sockfd()
,_epollfd(_sockfd.fd())
{
	_sockfd.ready(_inetAddr);
}
void TcpServer::start()
{
	_epollfd.loop();
}
void TcpServer::stop()
{
	_epollfd.unloop();
}
//回调函数执行
void TcpServer::setConnetionCallback(TcpServerCallback cb)
{
	_epollfd.setConnectionCallback(cb);
}
void TcpServer::setMessageCallback(TcpServerCallback cb)
{
	_epollfd.setMessageCallback(cb);
}
void TcpServer::setCloseCallback(TcpServerCallback cb)
{
	_epollfd.setCloseCallback(cb);
}
}//end of namespace wd
