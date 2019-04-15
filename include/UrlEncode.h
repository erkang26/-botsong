// FileName:UrlEncode.h
//
// Desc:
//
// Created by token.tong at 2018-01-24 14:14:38
#ifndef _URL_ENCODE_H_
#define _URL_ENCODE_H_
#include "Utils.h"
#include "sharelib.h"

class UrlEncode
{
public:
	UrlEncode();
	~UrlEncode();

	static string encode( const string& str );
	static string decode( const string& str );
};

#endif //_URL_ENCODE_H_
