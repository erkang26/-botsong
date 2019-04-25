// FileName:Cout.h
//
// Desc:
//
// Created by token.tong at 2019-04-17 14:18:19
#ifndef _COUT_H_
#define _COUT_H_
#include "Utils.h"
#include "sharelib.h"

struct Endl 
{
};

class Cout
{
public:
	Cout();
	~Cout();

	Cout& operator<<( bool b );
	Cout& operator<<( char c );
	Cout& operator<<( BYTE by );
	Cout& operator<<( short s );
	Cout& operator<<( WORD w );
	Cout& operator<<( int n );
	Cout& operator<<( DWORD dw );
	Cout& operator<<( long l );
	Cout& operator<<( ULONG ul );
	Cout& operator<<( float f );
	Cout& operator<<( double db );
	Cout& operator<<( const char* s );
	Cout& operator<<( const string& s );
	Cout& operator<<( Endl& el );
};

extern Cout COUT;
extern Endl ENDL;

#endif //_COUT_H_
