// FileName:NovBqg.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 09:24:08
#ifndef _NOV_BQG_H_
#define _NOV_BQG_H_
#include "Utils.h"
#include "sharelib.h"
#include "NovBase.h"

class TsMap;
class NovBqg : public NovBase
{
public:
	NovBqg();
	~NovBqg();

	string parseContent( const string& data );
	bool parseList( const string& data, map<string,string>& mapUrl );

	bool parseWebUrl( const string& data, map<string,string>& url );
	bool parseImgUrl( const string& data, map<string,string>& url );
	string getContent( const string& data );
	void save( const string& url, const string& dir, const string& data );

private:
	TsMap* _mapUrl;
};

#endif //_NOV_BQG_H_
