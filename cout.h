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
	Cout( const string& file, bool debug );
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

private:
	bool canOutput();
	void log( const string& data );

private:
	string _file;
	string _dir;
	bool _debug;
};

extern Cout COUT;
extern Endl ENDL;
extern Cout CERR;
extern Cout CSTAT;
extern Cout CRUN;
extern Cout CEXCEPT;
extern bool _need_daemon;

#endif //_COUT_H_
