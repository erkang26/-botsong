// FileName:UrlDelegate.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 15:05:11
#ifndef _URL_DELEGATE_H_
#define _URL_DELEGATE_H_
#include "Utils.h"
#include "sharelib.h"

enum URL_TYPE
{
	UT_NONE = 0,
	UT_COMMON = 1,
	UT_IMAGE = 2,
	UT_NT_BQG_LIST = 3,
	UT_NT_BQG_CONTENT = 4,
	UT_NT_BQG_BK = 5
};

class Url;
class UrlDelegate
{
public:

	static UrlDelegate* getInstance( URL_TYPE type );

	virtual string parseHtml( Url* parentUrl, const string& data ) { return ""; }
	virtual void save( Url* url, const string& dir, const string& data ) {}
};

#endif //_URL_DELEGATE_H_
