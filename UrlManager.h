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
	void addUrl( Url* url, bool force = false );
	void addImg( Url* url, bool force = false );

	long getNextSn();

	void print();

	void setSameSource( const string& url );

	void setDebug( bool debug ) { _debug = debug; }
	bool isDebug() const { return _debug; }

	void loadUrl( const string& path );
	void loadImg( const string& path );
	bool saveUrl( const string& path );
	bool saveImg( const string& path );

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
