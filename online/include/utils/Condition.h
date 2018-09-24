 ///
 /// @file    Condition.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-24 20:50:59
 ///
#ifndef _WD_CONDITION_H
#define _WD_CONDITION_H

#include "Noncopyable.h"
#include <pthread.h>
namespace wd
{
class MutexLock;

class Condition:Noncopyable
{
public:
	Condition(MutexLock &mutex);
	~Condition();

	void wait();
	void notify();
	void notifyall();
private:
	pthread_cond_t	_cond;
	MutexLock	&_mutex;
};
}
#endif
