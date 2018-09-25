 ///
 /// @file    WordSegmentation.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-25 09:50:18
 ///
 //创建分词工具
#ifndef _WD_WORD_SEGMENTATION_H
#define _WD_WORD_SEGMENTATION_H

#include "cppjieba/Jieba.hpp"
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::string;
using std::vector;
namespace wd
{
const char *const RICT_PATH="/home/yxg/cppjieba/dict/jieba.dict.utf8";
const char *const HMM_PATH="/home/yxg/cppjieba/dict/hmm_model.utf8";
const char *const USER_DICT_PATH="/home/yxg/cppjieba/dict/user.dict.utf8";

class WordSegmentation
{
WordSegmentation()
:_jieba(RICT_PATH,HMM_PATH,USER_DICT_PATH)
{
	cout<<"cppjieba init!"<<endl;
}
vector<string> operator()(const char *src)
{
	vector<string> words;
	_jieba.Cut(src,words,true);
	return woeds;
}
private:
	cppjieba::Jieba _jieba;
};
}
#endif
