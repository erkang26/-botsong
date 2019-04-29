// FileName:HtmlCommon.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 17:19:03
#ifndef _HTML_COMMON_H_
#define _HTML_COMMON_H_
#include "Utils.h"
#include "sharelib.h"
#include "UrlDelegate.h"

class HtmlCommon : public UrlDelegate
{
public:
	HtmlCommon();
	~HtmlCommon();

	string parseHtml( Url* parentUrl, const string& data );
	void save( Url* url, const string& dir, const string& data );

private:
	string getImageName( string url );
	string getFileByUrl( const string& url );
	void parseWebUrl( const string& domainUrl, const string& data );
	void parseImgUrl( const string& domainUrl, const string& data );
};

#endif //_HTML_COMMON_H_
