// FileName:UrlManager.h
//
// Desc:
//
// Created by token.tong at 2019-04-16 16:07:18
#ifndef _URL_MANAGER_H_
#define _URL_MANAGER_H_
#include "Utils.h"
#include "sharelib.h"

class UrlManager
{
public:
	~UrlManager();

	static UrlManager* getInstance();
	void parseWebUrl( const string& url, const string& data );
	void parseImgUrl( const string& url, const string& data );

	bool popUrl( string& url );
	bool popImg( string& url );
	void addUrl( const string& url );

	long getNextSn();

private:
	UrlManager();

	void addImg( const string& url );
	void init();
	string getDomain( const string& url );
	void formatUrl( const string& domain, string& url );


private:
	map<string, bool> _mapUsedUrl;
	list<string> _urlList;
	list<string> _imgList;

	pthread_mutex_t _mapLocker;
	pthread_mutex_t _urlLocker;
	pthread_mutex_t _imgLocker;
	pthread_mutex_t _snLocker;
	long _sn;
};

#endif //_URL_MANAGER_H_
