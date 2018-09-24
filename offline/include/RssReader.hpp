 ///
 /// @file    RssReader.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 19:04:35
 ///
#ifndef __WANGDAO_RSSREADER_H__
#define __WANGDAO_RSSREADER_H__

#include "tinyxml2.h"
#include <string>
#include <vector>
#include <memory>

using std::string;
using std::vector;
using std::shared_ptr;
using namespace tinyxml2;
struct RssItem//rss
{
	string _title;//标题
	string _link;//连接
	string _pubDate;//日期
	string _content;//内容
};
struct RssData//rss的数据结构
{
	string _version;//说明
	string _title;//标题
	string _link;//连接
	string _description;//概要
	string _language;//语言
	vector<shared_ptr<RssItem> > _items;//指向每一条消息的指针
};
class RssReader
{
public:
	RssReader(vector<string> &files);//读取容器中存放文件
	~RssReader();

	void loadFeedFile(const string &filename);
	void initWithRssString(const string &rss);
	void makePages(vector<string> &pages);
private:
	void parseRss(XMLDocument &doc);
private:
	vector<shared_ptr<RssItem> > _items;//指针容器，存放RssItem
	vector<string> &_files;//存放文件的容器
}
