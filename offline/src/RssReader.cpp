 ///
 /// @file    RssReader.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 19:30:45
 ///
#include "RssReader.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/regex.hpp>

using std::cout;
using std::endl;
using std::ostringstream;
using std::ofstream;

RssReader::RssReader(vector<string> &files)
:_files(files)
{
	cout<<"RssReader()"<<endl;
}
RssReader::~RssReader()
{
	cout<<"~RssReader()"<<endl;
}
void RssReader::loadFeedFiles()//处理容器中的每一个文件
{
	for(size_t idx=0;idx!=_files.size();++idx)
	{
		loadFeedFile(_files[idx]);
		cout<<"has load"<<idx+1<<"rss files"<<endl;
	}
}
//处理一个rss文件；拿到一个html文件先进行处理，再执行makePages()
void RssReader::loadFeedFile(const string &filename)
{
	XMLDocument doc;
	doc.LoadFile(filename.c_str());//加载html文件
	if(errorID)
	{
		cout<<"XMLDocument LoadFile error!"<<endl;
	}
	else
	{
		parseRss(doc);//解析html文件
	}
}
//把经过解析的html文件存放到容器中，即产生网页库。
void RssReader::makePages(vector<string> &pages)//生成网页库
{
	cout<<"pages's size="<<_items.size()<<endl;
	for(size_t idx=0;idx!=_items.size();++idx)
	{
		ostringstream oss;
		oss<<"<doc>\n"
			<<" <docid>" <<(idx+1)<<"</docid>\n"
			<<" <title>" <<_items[idx]->_title<<"</title>\n"
			<<" <link>" <<_items[idx]->_link<<"</link>\n"
			<<" <content>" <<_items[idx]->_content<<"</content>\n"
			<<"/<doc>\n";
		string page=oss.str();
		pages.push_back(page);//循环读取内容并插入到网页库中
	}
}
//解析html文件
void RssReader::parseRss(XMLDocument &doc)
{
	cout<<"parsRss(XMLDocument &doc)"<<endl;
	XMLElement *root=doc.FirstChildElement();//根节点
	//<channel>
	XMLElement *channel=root->FirstChildElement("channel");
	//<item>
	XMLElement *item=channel->FirstChildElement("item");
	for(;item;item=item->NextSiblingElement())//用迭代器循环获取item节点
	{
		shared_ptr<RssItem> rssItemPtr(new RssItem);//创建一个RessItem类型的智能指针
		//智能指针指向的消息体包含标题、连接、概述、内容
		//下面是获得一个消息的标题、连接、概述、内容，然后生成一个消息体
		//存放到_items容器中
		const char *itemTitle=item->FirstChildElement("title")->GetText();
		const char *itemlink=item->FirstChildElement("link")->GetText();
		const char *itemDescription=item->FirstChildElement("description")->GetText();
		const char *iContent=nullptr;
		XMLElement *contentEncoded=item->FirstChildElement("content:encoded");//结束
		if(contentEncoded)
		{
			iContent=contentEncoded->GetText();
		}
		else
		{
			iContent=itemDescription;
		}
		string szReg="<.*?>";//正则表达式去除<>
		//用到boost库中的regex_replace()函数
		boost::regex fmt(szReg);
		string content=boost::regex_replace(string(iContent),fmt,string(""));

		rssItemPtr->_title=itemTitle?itemTitle:string("");
		rssItemPtr->_link=itemLink?itemLink:string("");
		rssItemPtr->_content=content;

		_items.push_back(rssItemPtr);//处理过的html文件放入_item中
	}								//下一步进行makePages()
}

