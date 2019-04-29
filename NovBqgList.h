// FileName:NovBqgList.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 15:10:27
#ifndef _NOV_BQG_LIST_H_
#define _NOV_BQG_LIST_H_
#include "Utils.h"
#include "sharelib.h"
#include "NovBase.h"

class NovBqgList : public NovBase
{
public:
	NovBqgList();
	~NovBqgList();

	string parseHtml( Url* parentUrl, const string& data );
	void save( Url* url, const string& dir, const string& data );
};

#endif //_NOV_BQG_LIST_H_
