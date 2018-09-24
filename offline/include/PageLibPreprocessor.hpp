 ///
 /// @file    PageLibPreprocessor.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 23:42:24
 ///
#ifndef _WD_PAGE_LIB_PREPROCESSOR_H_
#define _WD_PAGE_LIB_PREPROCESSOR_H_

#include "WebPage.hpp"
#include "WordSegmentation.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::pair;
using std::map;
using std::unordered_map;
using std::set;

using namespace wd;

namespace wd
{

class PageLibPreprocessor
{
public:
	PageLibPreprocessor(Configuration &conf);//构造函数
	void doProcess();//执行预处理
private:
	void readInfoFromFile();//读取PageLib类中生成的网页库文件，
	void cutRedundanPages();//对冗余的网页进行去重
	void buildInverIndexTable();//创建倒排索引
	void storeOnDisk();//将处理后的网页库，位置偏移库和倒排索引库写回到磁盘上
private:
	Configuration		&_conf;//配置文件对象的引用
	WordSegmentation	_jieba;//分词对象
	vector<WebPage>		_pageLib;//网页库的容器对象
	unordered_map<int,pair<int,int> >	_offsetLib;//网页偏移库对象
	unordered_map<string,vector<pair<int,double>>>	_invertIndexTable;//倒排索引对象
};
}
#endif
