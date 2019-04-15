/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.
*/

#ifndef _SOCK_H_
#define _SOCK_H_
#include "Utils.h"
#include "sharelib.h"

class Sock
{
public:
	static SOCK_T connect( const string& host, WORD port );
	static SOCK_T create( const string& host, WORD port, bool TCP = true );
	static SOCK_T accept( SOCK_T sock );

	static int send( SOCK_T sock, const BYTE* data, size_t len );
	static int recv( SOCK_T sock, string& data );

	static int recvfrom( SOCK_T sock, string& data );

	static void close( SOCK_T sock );
};



#endif //_SOCK_H_
