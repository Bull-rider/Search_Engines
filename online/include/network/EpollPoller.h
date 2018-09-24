 ///
 /// @file    EpollPoller.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 22:28:16
 ///
#ifndef _WD_EPOLLPOLLER_H
#define _WD_EPOLLPOLLER_H

#include "Noncopyable.h"
#include "TcpConnection.h"
#include "MutexLock.h"
#include <sys/epoll.h>
#include <vector>
#include <map>
#include <functional>

namespace wd
{
class EpollPoller:Noncopyable
{
public:
	typedef TcpConnection::TcpConnectionCallback EpollCallback;//起别名
	typedef std::function<void()> Functor;//函数回调重定义

	EpollPoller(int listenfd);
	~EpollPoller();
	void loop();//执行事件循环
	void unloop();//退出事件循环
	void runInLoop(const Functor &cb);//向IO线程发送数据
	void doPendingFunctors();//

	void wakeup();//激活_eventfd(执行写操作)
	void handleRead();//处理_eventfd(执行读操作)
	//设置回调函数
	void setConnectionCallback(EpollCallback cb);
	void setMessageCallback(EpollCallback cb);
	void setCloseCallback(EpollCallback cb);

private:
	void waitEpollfd();//执行事件循环，由loop调用
	void handleConnecton();//处理连接
	void handleMessage(int peerfd);//处理旧连接
private:
	int _epollfd;//epoll实例的文件描述符
	int _listenfd;//服务器监听文件描述符
	int _wakeupfd;//用于线程间通信
	bool _isLooping;//标记是否进行循环

	MutexLock _mutex;
	std::vector<Functor> _pendingFunctors;//需要延迟的回调函数

	typedef std::vector<struct epoll_event> EventList;
	EventList _eventsList;//存放epoll中触发的事件
	typedef std::map<int,TcpConnetionPtr> ConnectionMap;
	ConnectonMap _connMap;//保存所有已建立的连接

	//回调函数，传递给TcpConnection对象
	EpollCallback _onConnectionCb;
	EpollCallback _onMessageCb;
	EpollCallback _onCloseCb;
};
}
#endif
