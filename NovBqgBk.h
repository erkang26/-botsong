// FileName:NovBqgBk.h
//
// Desc:
//
// Created by token.tong at 2019-04-29 10:43:45
#ifndef _NOV_BQG_BK_H_
#define _NOV_BQG_BK_H_
#include "Utils.h"
#include "sharelib.h"
#include "NovBase.h"

class NovBqgBk : public NovBase
{
public:
	NovBqgBk();
	~NovBqgBk();

	string parseHtml( Url* parentUrl, const string& data );
	void save( Url* url, const string& dir, const string& data );

private:
	void parseBk( Url* parentUrl, const string& data, const string& charset );
	void parsePg( Url* parentUrl, const string& data, const string& charset );
};

#endif //_NOV_BQG_BK_H_
