 ///
 /// @file    Buffer.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 22:19:15
 ///
#ifndef _WD_BUFFER_H
#define _WD_BUFFER_H

#include "Noncopyable.h"
#include "MutexLock.h"
#include "Condition.h"
#include <queue>
#include <functional>

namespace wd
{
class Buffer:Noncopyable
{
public:
	typedef std::function<void()> Task;
	Buffer(size_t size);

	void push(Task task);
	Task pop();

	bool full();
	bool empty();
	void wakeup_empty();
	void set_flag(bool flag);
private:
	MutexLock		_mutex;
	Condition		_notFull;
	Condition		_notEmpty;
	size_t			_size;
	std::queue<Task>	_que;
	bool			_flag;
};
}
#endif
