// FileName:HtmlDelegate.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 11:29:03
#ifndef _HTML_DELEGATE_H_
#define _HTML_DELEGATE_H_
#include "Utils.h"
#include "sharelib.h"

class HtmlDelegate
{
public:
	virtual bool parseWebUrl( const string& data, map<string,string>& url ) = 0;
	virtual bool parseImgUrl( const string& data, map<string,string>& url ) = 0;
	virtual string getContent( const string& data ) = 0;
	virtual void save( const string& url, const string& dir, const string& data ) = 0;
};

#endif //_HTML_DELEGATE_H_
