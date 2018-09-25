 ///
 /// @file    PageLib.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 17:25:39
 ///
#ifndef _WD_PAGELIB_H_
#define _WD_PAGELIB_H_
#include <string>
#include <vector>

namespace wd
{
class Configuration;
class DirScanner;
class PageLib
{
public:
	PageLib(Configuration &conf,DirScanner &dirScanner);
	void create();
	void store();
private:
	Configuration &_conf;
	DirScanner &_dirScanner;
	std::vector<std::string> _vecPages;//存放经过类RssReader中函数解析的网页item
};
}
#endif
