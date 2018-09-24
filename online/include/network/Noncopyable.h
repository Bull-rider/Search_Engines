 ///
 /// @file    Noncopyable.h
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-23 16:44:02
 ///
#ifndef _WD_NONCOPYABLE_H
#define _WD_NONCOPYABLE_H

namespace wd
{
class Noncopyable
{
public:
	Noncopyable();
private:
	Noncopyable(const Noncopyable &rhs);
	Noncopyable &operator=(const Noncopyable &rhs);
};
}
#endif
