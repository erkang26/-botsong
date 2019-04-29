// FileName:NovBase.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 09:39:43
#ifndef _NOV_BASE_H_
#define _NOV_BASE_H_
#include "Utils.h"
#include "sharelib.h"
#include "UrlDelegate.h"
#include "Locker.h"

class NovBase : public UrlDelegate 
{
public:
	NovBase();
	virtual ~NovBase();

protected:
	string getCharset( const string& data );
	string codeTrans( const string& charset, const string& data, const char* ex = "" );
};

#endif //_NOV_BASE_H_
