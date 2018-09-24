 ///
 /// @file    Socket.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 16:33:26
 ///
#ifndef _WD_SOCKET_H
#define _WD_SOCKET_H
#include "Noncopyable.h"

namespace wd
{
class InetAddress;
class Socket:Noncopyable
{
public:
	Socket(int sockfd);
	Socket();
	~Socket();
	void ready(const InetAddress &addr);
	int accept();
	void shutdownWrite();
	int fd()
	{return _sockfd;}
	static InetAddress getLocalAddr(int sockfd);
	static InetAddress getPeerAddr(int sockfd);
private:
	void bindAddress(const InetAddress &addr);
	void listen();
	void setReuseAddr(bool flag);
	void setReusePort(bool flag);
private:
	int _sockfd;
};
}
#endif
