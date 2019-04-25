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

class UrlManager
{
public:
	~UrlManager();

	static UrlManager* getInstance();
	void parseWebUrl( const string& domainUrl, const string& data );
	void parseImgUrl( const string& domainUrl, const string& data );

	bool popUrl( string& url );
	bool popImg( string& url );
	void addUrl( const string& url );
	void addImg( const string& url );

	long getNextSn();

	void print();

	void setSameSource( const string& url );

	void setDebug( bool debug ) { _debug = debug; }
	bool isDebug() const { return _debug; }

private:
	UrlManager();

	void init();
	string getDomain( const string& url );
	string getDomainWithDir( const string& url );
	void formatUrl( const string& domain, const string& doaminWithDir, string& url );


private:
	map<string, bool> _mapUsedUrl;
	list<string> _urlList;
	list<string> _imgList;

	pthread_mutex_t _mapLocker;
	pthread_mutex_t _urlLocker;
	pthread_mutex_t _imgLocker;
	pthread_mutex_t _snLocker;
	long _sn;
	string _sameSrc;
	bool _debug;
};

#endif //_URL_MANAGER_H_
