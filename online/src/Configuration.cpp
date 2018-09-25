 ///
 /// @file    Configuration.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-25 09:10:59
 ///
#include "Configuration.hpp"
#include "GlobalDefine.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

namespace wd
{
Configuration::Configuration(const std::string &filepath)
:_filepath(filepath)
{
	readConfiguration();
}
void Configuration::readConfiguration()
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
std::map<st::string,std::string> &Configuration::getConfigMap()
{
	return _configMap;
}
std::set<std::string> &Configuration::getStopWordList()
{
	if(_stopWordList.size()>0)
		return _stopWordList;
	std::string stopWordFilePath=_configMap[STOP_WORD_KEY];
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
}

