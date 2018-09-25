 ///
 /// @file    WordQuery.cpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-25 14:12:15
 ///
#include "WordQuery.hpp"
#include "GlobalDefine.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <json/json.h>

using std::cout;
using std::endl;
using std::ifstream;
using std::stringstream;

namespace wd
{
struct SimilarityCompare
{
	SimilarityCompare(vector<double> &base)
	:_base(base)
	{}
	bool operator()(const pair<int,vector<double> > &lhs,
			const pair<int,vector<double> > &rhs)
	{
		//都与基准向量进行计算
		double lhsCrossProduct=0;
		double rhsCrossProduct=0;
		double lhsVectorLength=0;
		double rhsVectorLength=0;

		for(int index=0;index!=_base.size();++index)
		{
			lhsCrossProduct+=(lhs.second)[index]*_base[index];
			rhsCrossProduct+=(lhs.second)[index]*_base[index];
			lhsVectorLength+=pow((lhs.second)[index],2);
			rhsVectorLength+=pow((lhs.second)[index],2);
		}
		if(lhsCrossProduct/sqrt(lhsVectorLength)<rhsCrossProduct/sqrt(rhsVectorLength))
		{
			return false;
		}
		else
		{return true;}
	}
	vector<double> _base;
};
WordQuery::WordQuery(Configuration &conf)
:_conf(conf)
{
	loadLibrary();
}
void WordQuery::loadLibrary()
{
	ifstream ifsOffset(_conf.getConfigMap()[NEWOFFSETLIB_KEY].c_str());
	ifstream ifsPage(_conf.getConfigMap()[NEWPAGELIB_KEY].c_str());

	if(!(ifsOffset.good()&&ifsPage.good()))
	{
		cout<<"offset page lib ifstream open error!"<<endl;
	}
	int docid,offset,length;
	string line;
	int cnt=0;
	while(getline(ifsOffset,line))
	{
		stringstream ss(line);
		ss>>docid>>offset>>length;

		string doc;
		doc.resize(length,' ');
		ifsPage.seekg(offset,ifsPage.beg());
		ifsPage.read(&*doc.begin(),length);

		WebPae webPage(doc,_conf,_jieba);
		_pageLib.insert(make_pair(docid,webPage));
		_offsetLib.insert(make_pair(docid,make_pair(offset,length)));
	}
	ifsOffset.close();
	ifsPage.close();
	//加载 索引表
	ifstream ifsInvertIndex(_conf.getConfigMap()[INVERTINDEX_KEY].c_str());
	if(!ifsInvertIndex.good())
	{
		cout<<"incert index ifstream open error!"<<endl;
	}
	string word;
	double weight;
	cnt=0;
	while(getline(ifsInvertIndex,line))
	{
		stringstream ss(line);
		ss>>word;
		set<pair<int,double> > setID;
		while(ss>>docid>>weight)
		{
			setID.insert(make_pair(docid,weight));
		}
		_invertIndexTable.insert(make_pair(word,setID));
	}
	ifsInvertIndex.close();
	cout<<"loadLibrary() end"<<endl;
}
string WordQuery::doQuery(const string &str)
{
	//对输入的字符串进行分词
	vector<string> queryWords;
	if(str.size()>0)
	{
		queryWords=_jieba(str.c_str());
	}
	for(auto item:queryWords)
	{
		cout<<item<<'\n';
		auto uit=_invertIndexTable.find(item);
		if(uit==_invertIndexTable.end())
		{
			cout<<"can not found"<<item<<endl;
			return returnNoAnswer();
		}
	}
	cout<<endl;
	//计算每个词的权重
	vector<double> weightList=getQueryWordsWeightVector(queryWords);
	SimilarityCompare similarityCmp(weightList);
	//执行查询
	vector<pair<int,vector<double> > > resultVec;
	if(executeQuery(queryWords,resultVec))
	{
		stable_sort(resultVec.begin(),resulVec.end(),similarityCmp);
		vector<int> docIdVec;
		for(auto item:resultVec)
		{
			docIdVec.push_back(item.first);
		}
		return createJson(docIdVec,queryWords);
	}
	else
	{
		return returnNoAnswer();
	}
}
string WordQuery::returnNoAnswer()
{
	Json::Value root;
	Json::Value arr;

	Json::Value elem;
	elem["title"]="404,not found";
	elem["summary"]="亲,I cannot find what you want. What a pity!";
	elem["url"]="";
	arr.append(elem);
	root["files"]=arr;
	Json::StyledWriter writer;
	return writer.write(root);
}
vector<double> WordQuery::getQueWordsWeightVector(vector<string> &queryWords)
{
	//统计词频
	map<string,int> wordFreqMap;
	for(auto item:queryWords)
	{
		++wordFreqMap[item];
	}
	//计算权重
	vector<double> weightList;
	double weightSum=0;
	int totalPageNum=_offsetLib.size();
	for(auto &item:queryWords)
	{
		int df=_invertIndexTable[item].size();
		double idf=log(static_cast<double>(totalPageNum)/df+0.05)/log(2);
		int tf=wordFreqMap[item];
		double w=idf*tf;
		weightSum+=pow(w,2);
		weightList.push_back(w);
	}
	//归一化
	for(auto &item:weightList)
	{
		item/=sqrt(weightSum);
	}
	cout<<"weightList's elem:";
	for(auto item:weightList)
	{
		cout<<item<<"\t";
	}
	cout<<endl;
	return weightList;
}

//执行查询
bool WordQuery::executeQuery(const vector<string> &queryWords,
		vector<pair<int,vector<double> > > &resultVec)
{
	cout<<"executeQuery()"<<endl;
	if(queryWords.size()==0)
	{
		cout<<"empty string not find"<<endl;
		return false;
	}
	typedef set<pair<int,double> >::iterator setIter;
	vector<pair<setIter,int> > iterVec;
	int minIterNum=0x7FFFFFFF;
	for(auto item:queryWords)
	{
		int sz=_invertIndexTable[item].size();
		if(sz==0)
			return false;
		if(minIterNum>sz)
			minIterNum=sz;
		iterVec.push_back(make_pair(_invertIndexTable[item].begin(),0));
	}
	cout<<"minIterNum="<<minIterNum<<endl;
	bool isExiting=false;
	while(!isExiting)
	{
		int idx=0;
		for(;idx!=iterVec.size()-1;++idx)
		{
			if((iterVec[idx].first)->first!=iterVec[idx+1].first->first)
				break;
		}
		if(idx==iterVec.size()-1)
		{
			//找到相同的docid
			vector<double> weightVec;
			int docid=iterVec[0].first->first;
			for(idx=0;idx!=iterVec.size();++idx)
			{
				weightVec.push_back(iterVec[idx].first->second);
				++(iterVec[idx].first);
				++(iterVec[idx].second);
				if(iterVec[idx].second==minIterNum)
				{
					isExiting=true;
				}
			}
			resultVec.push_back(make_pair(docid,weightVec));
		}
		else
		{
			int minDocId=0x7FFFFFFF;
			int iterIdex;
			for(idx=0;idx!=iter!=iterVec.size();++idx)
			{
				if(iterVec[idx].first->first<minDocId)
				{
					minDocId=iterVec[idx].first->first;
					iterIdx=idx;
				}
			}
			++(iterVec[iterIdx].first);
			++(iterVec[iterIdx].second);
			if(iterVec[iterIdx].second==minIterNum)
			{isExiting=true;}
		}
	}
	return true;
}
string WordQuery::createJson(vector<int> &docIdVec,const vector<string> &queryWords)
{
	Json::Value root;
	Json::Value arr;
	int cnt=0;
	for(auto id:docIdVec)
	{
		string summary=_pageLib[id].summary(queryWords);
		string title=_pageLib[id].getTitle();
		string url=_pageLib[id].getUrl();

		Json::Value elem;
		elem["title"]=title;
		elem["summary"]=summary;
		elem["url"]=url;
		arr.append(elem);
		if(++cnt==100)
			break;
	}
	root["files"]=arr;
	Json::StyledWriter writer;
	return writer.write(root);
}
}
