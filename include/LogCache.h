/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.
*/

#ifndef _LOG_CACHE_H_
#define _LOG_CACHE_H_
#include "Utils.h"
#include "sharelib.h"

class EndLine
{
};

static EndLine el;

class LogCache
{
public:
	LogCache();
	~LogCache();

	LogCache& operator<<( const char* s );
	LogCache& operator<<( const string& s );
	LogCache& operator<<( int n );
	LogCache& operator<<( long n );
	LogCache& operator<<( char c );
	LogCache& operator<<( short s );
	LogCache& operator<<( BYTE b );
	LogCache& operator<<( WORD w );
	LogCache& operator<<( DWORD dw );
	LogCache& operator<<( ULONG ul );
	LogCache& operator<<( float f );
	LogCache& operator<<( double db );
	LogCache& operator<<( void* p );
	LogCache& operator<<( const EndLine& e );
	LogCache& operator<<( const LogCache& ) { return *this; }

	void clear();

	const string& get() const { return _data; }

private:
	string _data;
};


#endif //_LOG_CACHE_H_
