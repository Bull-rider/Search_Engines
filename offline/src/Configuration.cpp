 ///
 /// @file    Configuration.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 14:57:09
 ///
#include "Configuration.hpp"
#include "GlobalDefine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;

namespace wd
{
Configuration::Configuration(const std::string &filepath)
:_filepath(filepath)
{
	readConfiguration();
}
void Configuration::readConfiguration()//读取配置文件信息并存放到map中
{
	std::ifstream ifs(_filepath.c_str());
	if(ifs.good())
	{
		std::string line;
		while(getline(ifs,line))
		{
			std::istringstream iss(line);
			std::string key;
			std::string value;
			iss>>key>>value;
			_configMap[key]=value;
		}
		std::cout<<"read configuration is over!"<<std::endl;
	}
	else
	{
		std::cout<<"ifstream open error"<<std::endl;
	}
}
std::map<std::string,std::string> &getfiguration::getConfigMap()
{
	return _configMap;
}
std::set<std::string> &Configuration::getStopWordList()//处理停用词
{
	if(_stopWordList.size()>0)
		return _stopWordList;
	std::string stopWordFilePath=_configMap[STOP_WORD_KEY];//拿到停用词文件所在的绝对路径
	std::ifstream ifs(stopWordFilePath.c_str());
	if(!ifs)
	{
		std::cout<<"StopWord ifstream open error!"<<std::endl;
	}
	std::string word;
	while(getline(ifs,word))
	{
		_stopWordList.insert(word);
	}
	return _stopWordList;
}
void Configuration::debug()
{
	for(auto &elem:_configMap)
	{
		std::cout<<elem.first<<"-->"<<elem.second<<std::endl;
	}
}
}
