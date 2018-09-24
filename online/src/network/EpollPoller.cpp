 ///
 /// @file    EpollPoller.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 15:13:54
 ///
#include "EpollPoller.cpp"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <sys.eventfd.h>

namespace wd
{
int createEpollFd()
{
	int epfd=::epoll_create1(0);
	if(epfd==-1)
	{
		perror("epoll_create1 error");
		exit(EXIT_FAILURE);
	}
	return epfd;
}
int createEventFd()
{
	int evtfd=::eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
	if(evtfd==-1)
	{
		perror("eventfd create error");
	}
	return evtfd;
}

void addEpollFdRead(int efd,int fd)//把文件描述符添加到epoll监控
{
	struct epoll_event ev;
	ev.data.fd=fd;
	ev.events=EPOLLIN;
	int ret=epoll_ctl(efd,EPOLL_CTL_ADD,fd,&ev);
	if(ret==-1)
	{
		perror("epoll_ctl add error");
		exit(EXIT_FAILURE);
	}
}
void delEpollReadFd(int efd,int fd)//从兴趣列表中删除一个文件描述符
{
	struct epoll_event ev;
	ev.data.fd=fd;
	int ret=epoll_ctl(efd,EPOLL_CTL_DEL,fd,&ev);
	if(ret==-1)
	{
		perror("epoll_ctl add error");
		exit(EXIT_FAILURE);
	}
}
int acceptConnFd(int listenfd)
{
	int peerfd=::accept(listenfd,NULL,NULL);
	if(peerfd==-1)
	{
		perror("accept error");
		exit(EXIT_FAILURE);
	}
	return peerfd;
}
//预览数据
size_t recvPeek(int sockfd,void *buf,size_t len)
{
	int nread;
	do
	{
		nread=::recv(sockfd,buf,len,MSG_PEEK);
	}while(nread==-1&&errno==EINTR);
	return nread;
}
//通过预览数据，判断conn是否关闭
bool isConnectionClosed(int sockfd)
{
	char buf[1024];
	int nread=recvPeek(sockfd,buf,sizeof(buf));
	if(-1==nread)
	{
		perror("recvPeek----");
		return true;
	}
	return (0==nread);
}

EpollPoller::EpollPoller(int listenfd)
:_epollfd(createEpollFd())
,_listenfd(listenfd)
,_wakeupfd(createEventFd())
,_isLooping(false)
,_eventsList(1024)
{
	addEpollFdRead(_epollfd,_listenfd);
	addEp0llFdRead(_epollfd,_wakeupfd);
}
EpollPoller::~EpollPoller()
{
	::close(_epollfd);
}
void EpollPoller::loop()
{
	_isLooping=true;
	while(_isLooping)
	{
		waitEpollfd();
	}
}
void EpollPoller::unloop()
{
	if(_isLooping)
		_isLooping=false;
}
void EpollPoller::runInLoop(const Functor &cb)
{
	{
		MutexLockGuard mlg(_mutex);
		_pendingFunctors.puch_back(cb);
	}
	wakeup();
}
void EpollPoller::doPendingFunctors()
{
	std::vector<Functor> functors;
	{
		MutexLockGuard mlg(_mutex);
		functors.swap(_pendingFunctors);
	}
	for(size_t i=0;i<functors.size();++i)
	{
		functors[i]();
	}
}
void EpollPoller::wakeup()
{
	uint64_t one=1;
	ssize_t n=::write(_wakeupfd,&one,sizeof(one));
	if(n!=sizeof(one))
	{
		perror("EpollPoller::wakeup() n!=8");
	}
}
void EpollPoller::handleRead()
{
	uint64_t one=1;
	ssize_t n=::read(_wakeupfd,&one,sizeof(one));
	if(n!=sizeof(one))
	{
		perror("EpollPoller::handleRead() n!=8");
	}
}
void EpollPoller::setConnectionCallback(EpollCallback cb)
{
	_onConnectionCb=cb;
}
void EpollPoller::setMessageCallback(EpollCallback cb)
{
	_onMessageCb=cb;
}
void EpollPoller::setCloseCallback(EpollCallback cb)
{
	_onCloseCb=cb;
}
void EpollPoller::waitEpollfd()
{
	int nready;
	do
	{
		nready=::epoll_wait(_epollfd,&(*_eventsList.begin()),
				_eventsList.size(),5000);
	}while(nready==-1&&errno==EINTR);
	if(nready==-1)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(nready==0)
	{
		printf("epoll_wait timeout\n");
	}
	else
	{
		//扩容
		if(nready==static_cast<int>(_eventsList.size()))
		{
			_eventsList.resize(_eventsList.size()*2);
		}
		//遍历每一个激活的文件描述符
		for(int idx=0;idx!=nready;++idx)
		{
			if(_eventsList[idx].data.fd==_listenfd)
			{
				if(_eventsList[idx].events&EPOLLIN)
				{
					handleConnection();
				}
			}
			else if(_eventsList[idx].data.fd==_wakeupfd)
			{
				printf("wakeupfd light\n");
				if(_eventsList[idx].events&EPOLLIN)
				{
					handleRead();
					doPendingFunctors();
				}
			}
			else
			{
				if(_eventsList[idx].events&EPOLLIN)
				{
					handleMessage(_eventsList[idx].data.fd);
				}
			}
		}
	}
}
void EpollPoller::handleConnection()
{
	int peerfd=acceptConnFd(_listenfd);
	addEpollFdRead(_epollfd,peerfd);
	TcpConnectionPtr conn(new TcpConnetion(peerfd,this));

	conn->setConnectionCallback(_onConnectionCb);
	conn->setMessageCallback(_onMessageCb);
	conn->setCloseCallback(_onCloseCb);

	std::pair<ConnectionMap::iterator,bool> ret;
	ret=_connMap.insert(std::make_pair(peerfd,conn));
	assert(ret.second==true);
	(void)ret;
	conn->handleConnectionCallback();
}
void EpollPoller::handleMessage(int peerfd)
{
	bool _isClosed=isConnectionClosed(peerfd);
	ConnectionMap::iterator it=_connMap.find(peerfd);
	assert(it!=_connMap.end());

	if(isClosed)
	{
		it->second->handleCloseCallback();
		delEpollReadFd(_epollfd,peerfd);
		_connMap.erase(it);
	}
	else
	{
		it->second->handleMessageCallback();
	}
}
}
