// FileName:Cookie.h
//
// Desc:
//
// Created by token.tong at 2019-04-15 17:54:08
#ifndef _COOKIE_H_
#define _COOKIE_H_
#include "Utils.h"
#include "sharelib.h"

class Cookie
{
public:
	Cookie();
	~Cookie();

	bool parse( const string& line );

	const string& getName() const { return _name; }
	const string& getValue() const { return _value; }
	const string& getExpires() const { return _expires; }
	const string& getDomain() const { return _domain; }
	const string& getPath() const { return _path; }
	bool isSecure() const { return _secure; }
	long getMaxAge() const { return _maxAge; }
	bool isHttpOnly() const { return _httpOnly; }
	time_t getExpiresTime() const { return _expiresTime; }

	void print() const;

private:
	void transTime();

private:
	string _name;
	string _value;
	string _expires;
	string _domain;
	string _path;
	bool _secure;
	long _maxAge;
	bool _httpOnly;
	time_t _expiresTime;
};

#endif //_COOKIE_H_
