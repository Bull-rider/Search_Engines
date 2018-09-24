 ///
 /// @file    MutexLock.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 16:44:25
 ///
#ifndef _WD_MUTEXLOCK_H
#define _WD_MUTEXLOCK_H
#include "Noncopyable.h"
#include <pthread.h>

namespace wd
{
class MutexLock:Noncopyable
{
public:
	MutexLock()
	:_isLocked(false)
	{pthread_mutex_init(&_mutex,NULL);}
	~MutexLock()
	{
		if(!_isLocked)
			pthread_mutex_destroy(&_mutex);
	}

	void lock()
	{
		pthread_mutex_lock(&_mutex);
		_isLocked=true;
	}
	void unlock()
	{
		pthread_mutex_unlock(&_mutex);
		_isLocked=false;
	}
	pthread _mutex_t *getMutexLockPtr()
	{
		return _mutex;
	}
private:
	pthread_mutex_t _mutex;
	bool _isLocked;
};
class MutexLockGuard
{
public:
	MutexLockGuard(MutexLoxk &mutex)
	:_mutex(mutex)
	{_mutex.lock();}
	~MutexLockGuard()
	{_mutex.unlock();}
private:
	MutexLock &_mutex;
};
}
