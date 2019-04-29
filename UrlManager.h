// FileName:UrlManager.h
//
// Desc:
//
// Created by token.tong at 2019-04-16 16:07:18
#ifndef _URL_MANAGER_H_
#define _URL_MANAGER_H_
#include "Utils.h"
#include "sharelib.h"
#include "cout.h"
#include "Locker.h"
#include "TsMap.h"
#include "TsList.h"

class Url;
class UrlManager
{
public:
	~UrlManager();

	static UrlManager* getInstance();

	Url* popUrl();
	Url* popImg();
	void addUrl( Url* url );
	void addImg( Url* url );

	long getNextSn();

	void print();

	void setSameSource( const string& url );

	void setDebug( bool debug ) { _debug = debug; }
	bool isDebug() const { return _debug; }

private:
	UrlManager();

	void init();


private:
	TsMap _mapUsedUrl;
	TsList _urlList;
	TsList _imgList;

	Locker _snLocker;
	long _sn;
	string _sameSrc;
	bool _debug;
};

#endif //_URL_MANAGER_H_
