 ///
 /// @file    Tcpserver.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 14:49:25
 ///
#ifndef _WD_TCPSERVER_H
#define _WD_TCPSERVER_H
#include "InetAddress.h"
#include "Socket.h"
#include "EpollPoller.h"

namespace wd
{
class TcpServer
{
public:
	typedef TcpConnection::TcpConnetionCallback TcpServerCallback;//取别名
	TcpServer(unsigned short port);
	TcpServer(const char *IP,unsigned short port);

	void start();//开始服务
	void stop();//停止服务
	//设置回调函数
	void setConnectionCallback(TcpServerCallback cb);
	void setMessageCallback(TcpServerCallback cb);
	void setCloseCallback(TcpServerCallback cb);
private:
	InetAddress		_inetAddr;//存放服务器的网络地址
	Socket			_sockfd;//Socket对象
	EpollPoller		_epollfd;//EpollPoller对象
	//回调函数，传递给EpollPoller对象
	TcpServerCallback	_onConnectionCb;
	TcpServerCallback	_onMessageCb;
	TcpServerCallback	_onCloseCb;

};
}
#endif
