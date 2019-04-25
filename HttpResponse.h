// FileName:HttpResponse.h
//
// Desc:
//
// Created by token.tong at 2019-04-15 15:57:46
#ifndef _HTTP_RESPONSE_H_
#define _HTTP_RESPONSE_H_
#include "Utils.h"
#include "sharelib.h"

class HttpRequest;
class Cookie;

class HttpResponse
{
public:
	HttpResponse( HttpRequest* request );
	~HttpResponse();

	bool parse( const string& data );

	int getCode() const { return _code; }
	const string& getBody() const { return _body; }
	const vector<Cookie*>& getCookies() const { return _cookies; }

	HttpRequest* getRequest() { return _request; }

	bool hasHeader( const string& key );
	const string& getHeader( const string& name );

private:
	bool parseHeader( const string& header );
	bool parseBody( const string& body );
	void parseFirstLine( const string& line );

private:
	HttpRequest *_request;
	map< string, string > _headers;
	string _body;
	string _ver;
	int _code;
	string _codeDesc;
	vector<Cookie*> _cookies;
	string _empty;
};

#endif //_HTTP_RESPONSE_H_
