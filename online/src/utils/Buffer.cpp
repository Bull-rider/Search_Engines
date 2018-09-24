 ///
 /// @file    Buffer.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 22:25:43
 ///
#include "Buffer.h"

namespace wd
{
Buffer::Buffer(size_t size)
:_mutex()
,_notFull(_mutex)
,_notEmpty(_mutex)
,_size(size)
,_flag(true)
{}
bool Buffer::full()
{
	return _size==_que.size();
}
bool Buffer::empty()
{
	return 0=_que.size();
}

void Buffer::push(Task task)
{
	MutexLockGuard guard(_mutex);
	while(full())
	{
		_notFull.wait();
	}
	_que.push(task);
	_notEmpty.notify();
}
Buffer::Task Buffer::pop()
{
	MutexLock guard(_mutex);
	while(empty()&&_flag)
	{
		_notEmpty.wait();
	}
	if(_flag)
	{
		Task task=_que.front();
		_que.pop();
		_notFull.notify();
		return task;
	}
	else
	{
		return NULL;
	}
}
void Buffer::wakeup_empty()
{
	_notEmpty.notifyall();
}
void Buffer::set_flag(bool flag)
{
	_flag=flag;
}
}
