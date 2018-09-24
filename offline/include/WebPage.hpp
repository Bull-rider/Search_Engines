 ///
 /// @file    WebPage.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 21:12:45
 ///
#ifndef _WD_WEB_PAGE_H_
#define _WD_WEB_PAGE_H_

#include "Configuration.hpp"
#include "WordSegmentation.hpp"

#include <string>
#include <map>
#include <set>
#include <vector>

using std::string;
using std::map;
using std::vector;
using std::set;

namespace wd
{
class WedPage
{
	//判断两篇文档是否相等
	friend bool operator==(const WebPage &lhs,const WebPage &rhs);
	//对文档按Docid进行排序
	friend bool operator<(const WsbPage &lhs,const Webpage &rhs)
public:
	const static int TOPK_NUMBER=20;

	WebPage(string &doc,Configuration &config,WordSegmentation &jieba);

	int getDocId()//获取文档id
	{return _docId;}
	map<string,int> &getWordsMap()//获取文档的词频统计
	{return _wordsMap;}
	string getDoc()//
	{return _doc;}
private:
	void processDoc(const string &doc,Configuration &config,WordSegmentation,&jieba);
	void calcTopK(vector<string> &wordsVec,int k,set<string> &stopWordList);
private:
	string	_doc;//整片文档，包含xml在内
	int		_docId;//文档id
	string	_docTitle;//文档标题
	string	_docUrl;//文档URL
	string	_docContent;//文档内容
	string	_docSummary;//文档摘要，需要自动生成，不是确定的

	vector<string>	_topWords;//存放最高词频的前20个词
	map<string,int>	_wordsMap;//存储文档的所有词（去停用词之后）和该项词的词频
};
}
#endif
