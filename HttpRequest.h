// FileName:HttpRequest.h
//
// Desc:
//
// Created by token.tong at 2019-04-15 15:22:21
#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_
#include "Utils.h"
#include "sharelib.h"
#include "Url.h"

class SocketClient;
class HttpResponse;

class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	HttpResponse* get( const string& url );

	void addHeader( const string& key, const string& value );
	void addHeader( const string& key, long value );
	void addHeader( const string& key, int value );
	void addHeader( const string& key, float value );

	void setBody( const string& body ) { _body = body; }

	const string& getOriginUrl() const { return _originUrl; }

private:
	void makeRequest( string& data, const string& method );
	void parseRequest( const string& url );
	bool doRequest( const string& method );

private:
	string _originUrl;
	SocketClient* _sock;
	HttpResponse* _response;
	map<string, string> _headers;
	string _body;
	string _host;
	WORD _port;
	bool _ssl;
	string _uri;
};

#endif //_HTTP_REQUEST_H_
