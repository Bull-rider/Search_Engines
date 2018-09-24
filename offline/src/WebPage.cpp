 ///
 /// @file    WebPage.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 22:03:19
 ///
#include "WebPage.hpp"
#include <stdlib.h>
#include <iostream>
#include <queue>
#include <algorithm>

using std::cout;
using std::endl;
using std::priority_queue;
using std::pair;
using std::make_pair;

namespace wd
{
struct WordFreqCompare//词频比较
{
	bool operator()(const pair<string,int> &left,const pair<string,int> &right)
	{
		if(left.second<right.second)
		{return true;}
		else if(left.second==right.second&&left.first>right.first)
		{return true;}
		else
		{return false;}
	}
};
WebPage::WebPage(string &doc,Configuration &config,WordSegmentation &jieba)
:_doc(doc)
{
	_topWords.reserve(10);
	processDoc(doc,config,jieba);//
}
void WebPage::processDoc(const string &doc,Configuration &config,WordSegmentation &jieba)
{
	string docIdHead="<docid>";
	string docIdTail = "</docid>";
	string docUrlHead = "<link>";
	string docUrlTail = "</link>";
	string docTitleHead = "<title>";
	string docTitleTail = "</title>";
	string docContentHead = "<content>";
	string docContentTail = "</content>";
	//提取文档的docid
	int bpos=doc.find(docIdHead);
	int epos=doc.find(docIdTail);
	string docId=doc.substr(bpos+docIdHead.size(),epos-bpos-docIdHead.size());
	_docId=atoi(docId.c_str());
	//tiele
	bpos=doc.find(docTitleHead);
	epos=doc.find(docTitleTail);
	_docTitle=doc.substr(bpos+docTitleHead.size(),epos-bpos-docTitleHead.size());
	//content
	bpos=doc.find(docContentHead);
	epos=doc.find(docContentTail);
	_docContent=doc.substr(bpos+docContentHead.size(),epos-bpos-docContentHead.size());
	//分词，把内容传进去，进行分词
	vector<string> wordsVec=jieba(_docContent.c_str());
	//读取停用词并存放到容器中
	set<string> &stopWordList=config.getStopWordList();
	//获取词频最高的前20个词
	calcTopK(wordsVec,TOPK_NUMBER,stopWordList);
}
//获取词频最高的前k个词
void WebPage::calcTop(vector<string> &wordsVec,int k,set<string> &stopWordList)
{
	for(auto iter=wordsVec.begin();iter!=wordsVec.end();++iter)
	{
		auto sit=stopWordList.find(*iter);
		if(sit==stopWordList.end())
		{
			++_wordsMap[*iter];
		}
	}
	priority_queue<pair<string,int>,vector<pair<string,int> >,WordFreqCompare>
		wordFreqQue(_wordsMap.begin(),_wordsMap.end());
	while(!wordFreqQue.empty())
	{
		string top=wordFreqQue.top().first;
		wordFreqQue.pop();
		if(top.size()==1&&(static_cast<unsigned int>(top[0])==10||
					static_cast<unsigned int>(top[0])==13))
		{continue;}
		_topWords.push_back(top);
		if(_topWords.size()>=static_cast<size_t(k))
		{
			break;
		}
	}
}
//判断两篇文档是否相同,用迭代器比较文档中的所有内容，相似度>=0.75
//视为两篇文档相同
bool operator==(const WebPage &lhs,const WebPage &rhs)
{
	int commNum=0;
	auto lIter=lhs._topWords.begin();
	for(;lIter!=lhs._topWords.end();++lIter)
	{
		commNum+=std::cout(rhs._topWords.begin(),rhs._topWords.end(),*lIter);
	}
	int lhsNum=lhs._topWords.size();
	int rhsNum=rhs._topWords.size();
	int totalNum=lhsNum<rhsNum?lhsNum:rhsNum;
	if(static_cast<double>(commNum)/totalNum>0.75)
	{return true;}
	else
	{return false;}
}
//对文档按照docId进行排序
bool operator<(const WebPage &lhs,const WebPage &rhs)
{
	if(lhs._docId<rhs._docId)
		return true;
	else
		return false;
}
}
