 ///
 /// @file    SocketIO.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 21:17:32
 ///
#ifndef _WD_SOCKETIO_H
#define _WD_SOCKETIO_H
#include <stdio.h>
namespace wd
{
class SocketIO
{
public:
	SocketIO(int sockfd);

	size_t readn(char *buf,size_t count);
	size_t writen(const char *buf,size_t count);
	size_t readline(char *buf,size_t mac_len);
private:
	size_t recv_peek(char *buf,size_t count);
private:
	int _sockfd;
};
}
#endif
