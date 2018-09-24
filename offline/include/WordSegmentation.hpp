 ///
 /// @file    WordSegmentation.hpp
 /// @author  lemon(haohb13@gmail.com)
 /// @date    2018-09-21 21:00:33
 ///
#ifndef _WD_WORD_SEGMENTATION_H_
#define _WD_WORD_SEGMENTATION_H_
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
//jieba分词工具的依赖文件
const char *const DICT_PATH="/home/yxg/cppjieba/dict/jieba.dict.utf8";
const char *const HMM_PATH="/home/yxg/cppjieba/dict/hmm_model.utf8";
const char *const USER_DICT_PATH="/home/yxg/cppjieba/dict/user.dict.utf8";
class WordSegmentation
{
public:
	WordSegmentation()
	:_jieba(DICT_PATH,HMM_PATH,USER_DICT_PATH)
	{
		cout<<"cppjieba init!"<<endl;
	}

	vector<string> operator()(const char *src)//传进来一个指向文件内容的指针
	{
		vector<string> words;
		_jieba.CutAll(src,word);//分词后存放到一个容器中
		return words;
	}
};
}
#endif
//分词工具的初始化操作
