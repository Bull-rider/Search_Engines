 ///
 /// @file    TcpConnection.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 19:06:12
 ///
 //当类A的成员函数需要把当前对象作为参数传给其他函数时就需要传递一个指向自身的
 //shared_ptr,我们就使用类A继承enable_shared_from_this,然后通过其成员函数
 //share_from_this()返回指向自身的shared_ptr
#ifndef _WD_TCPCONNECTION_H
#define _WD_TCPCONNECTION_H

#include "Noncopyable.h"
#include "InetAddress.h"
#include "Socket.h"
#include "SocketIO.h"

#include <string>
#include <memory>
#include <functional>

namespace wd
{
//前向声明
class EpollPoller;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
class TcpConnection:Noncopyable,public std::enable_shared_from_this<TcpConnection>
{
public:
	typedef std::function<void(const TcpConnectionPtr &)> TcpConnectionCallback;
	TcpConnection(int sockfd,EpollPoller *loop);
	~TcpConnection();

	std::string receive();//接收数据
	void send(const std::string &msg);//发送数据
	void sendAndClose(const std::string &msg);//发送数据，关闭连接(针对网页服务)
	void sendInLoop(const std::string &msg);//将数据交给IO线程发送
	void shutdown();//关闭连接

	std::string toString();//返回连接的字符串表示
	//设置回调函数
	void setConnectionCallback(TcpConnectionCallback cb);
	void setMessageCallback(TcpConnectionCallback cb);
	void setCloseCallback(TcpConnectionCallback cb);
	//调用相应的回调函数
	void handleConnectionCallback();
	void handleMessageCallback();
	void handleCloseCallback();
private:
	Socket _sockfd;//网络套接字类对象
	SocketIO _sockIO;//网络IO类对象
	const InetAddress _localAddr;//本地ip和端口信息
	const InetAddress _peerAddr;//远程ip和端口信息
	bool _isShutdownWrite;
	EpollPoller *_loop;//保存EpollPoller对象的指针
	//回调函数
	TcpConnectionCallback _onConnectionCb;
	TcpConnectionCallback _onMessageCb;
	TcpConnectionCallback _onCloseCb;
};
}
#endif
