// FileName:Url.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 14:23:46
#ifndef _URL_H_
#define _URL_H_
#include "Utils.h"
#include "sharelib.h"
#include "UrlDelegate.h"

#define HTTP_HEADER "http://"
#define HTTPS_HEADER "https://"

#define HTTP_HEADER_SIZE 7
#define HTTPS_HEADER_SIZE 8

class Url
{
public:
	Url();
	Url( const Url& obj );
	~Url();

	void setUrl( const string& parentUrl, const string& url );
	void setUrl( const string& url ) { _url = url; }
	const string& getUrl() const { return _url; }

	void setFlag( URL_TYPE flag ) { _flag = flag; }
	URL_TYPE getFlag() const { return _flag; }

	void setEx( const string& ex ) { _ex = ex; }
	const string& getEx() const { return _ex; }

	void addArr( const string& s ) { _arr.push_back(s); }
	const vector<string>& getArr() const { return _arr; }

	string serial();
	bool load( const string& data );

private:
	string getDomain( const string& url );
	string getDomainWithDir( const string& url );
	string formatUrl( const string& domain, const string& domainWithDir, const string& url );

	void reset();

	bool load1_0( const vector<string>& v );


private:
	string _url;
	URL_TYPE _flag;
	string _ex;
	vector<string> _arr;
};

#endif //_URL_H_
