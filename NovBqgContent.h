// FileName:NovBqgContent.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 16:59:09
#ifndef _NOV_BQG_CONTENT_H_
#define _NOV_BQG_CONTENT_H_
#include "Utils.h"
#include "sharelib.h"
#include "NovBase.h"

class NovBqgContent : public NovBase
{
public:
	NovBqgContent();
	~NovBqgContent();

	string parseHtml( Url* parentUrl, const string& data );
	void save( Url* url, const string& dir, const string& data );
};

#endif //_NOV_BQG_CONTENT_H_
