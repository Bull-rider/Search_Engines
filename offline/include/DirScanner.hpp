 ///
 /// @file    DirScanner.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 15:46:06
 ///
#ifndef _WD_DIRSCANNER_H_
#define _WD_DIRSCANNER_H_

#include <string>
#include <vector>
namespace wd
{
const int kFileNo=1000;
class Configuration;
class DirScanner
{
public:
	DirScanner(Configuration &conf);
	void operator()();//重载函数调用运算符，调用traverse函数
	std::vector<std::string> &files();//返回_vecFiles的引用
	void debug();
private:
	void traverse(const std::string &dirName);//获取某一目录下的所有文件
private:
	Configuration &_conf;//配置文件对象的引用
	std::vector<std::string> _vecFiles;//存放每个语料文件的绝对路径
};
}
#endif
