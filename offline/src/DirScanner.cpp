 ///
 /// @file    DirScanner.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 15:59:30
 ///
#include "DirScanner.hpp"
#include "Configuration.hpp"
#include "GlobalDefine.hpp"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>

using std::cout;
using std::endl;
namespace wd
{
DirScanner::DirScanner(Configuration &conf)
:_conf(conf)
{
	_vecFiles.reserve(kFileNo);
}
void DirScanner::operator()()
{
	std::map<std::string,std::string> &confMap=_conf.getConfigMap();
	std::string dirName=confMap[YULIAO_KEY];//拿到语料文件的路径
	traverse(dirName);//调用函数把语料目录下的文件的绝对路径存放到容器中
}						//下一步就要处理文件了。
std::vector<std::string> &DirScanner::files()
{
	return _vecFiles;
}
void DirScanner::debug()
{
	for(auto &elem:_vecFiles)
	{
		cout<<elem<<endl;
	}
}
void DirScanner::traverse(const std::string &dirName)
{
	DIR *pdir=::opendir(dirName.c_str());
	if(NULL==pdir)
	{
		printf("dir open error");
		exit(EXIT_FAILURE);
	}
	::chdir(dirName.c_str());
	struct dirent *mydirent;//存放目录信息
	struct stat mystat;//存放目录下文件的信息
	while(NULL!=(mydirent=::readdir(pdir)))//打开目录
	{
		::stat(mydirent->d_name,&mystat);//把目录下的文件名存放到mystat结构体中
		if(S_ISDIR(mystat.st_mode))//如果文件类型是目录，判断是不是.或..
		{
			if(0==::strncmp(mydirent->d_name,".",1)||
					0==::strncmp(mydirent->d_name,"..",2))
			{
				continue;
			}
			else//递归调用读取
			{
				traverse(mydirent->d_name);
			}
		}
		else
		{
			std::string fileName;
			//拼接文件的绝对路径
			fileName.append(::getcwd(NULL,0)).append("/").append(mydirent->d_name);
			_vecFiles.push_back(fileName);//把文件的绝对路径存放到容器中
		}
	}
	::chdir("..");
	::closedir(pdir);
}
}
