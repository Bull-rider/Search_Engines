 ///
 /// @file    Configuration.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-25 09:06:11
 ///
#ifndef _WD_CONFIGURATION_H
#define _WD_CONFIGURATION_H

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
	std::set<std::string> &getStopWordList();
private:
	void readConfiguration();
private:
	std::string	_filepath;
	std::map<std::string,std::string>	_configMap;
	std::set<std::string>	_stopWordfList;
};
}
#endif
