 ///
 /// @file    Configuration.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 14:50:37
 ///
#ifndef _WD_CONFIGURATION_H_
#define _WD_CONFIGURATION_H_

#include <set>
#include <map>
#include <string>

namespace wd
{
class Configuration
{
public:
	Configuration(const std::string &filepath);
	std::map<std::string,std::string> &getConfigMap();
	std::set<std::string> getStopWordList();
	void debug();
private:
	void readConfiguration();
private:
	std::string _filepath;
	std::map<std::string,std::string> _configMap;//存放配置文件中的文件的容器
	std::set<std::string> _stopWordList//存放停用词容器
};
}
#endif
