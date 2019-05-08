// FileName: Cout.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-17 14:18:19
#include "cout.h"
#include "UrlManager.h"

extern string CUR_DIR;

Cout COUT( "debug.log", true );
Endl ENDL;
Cout CERR( "error.log", false );
Cout CSTAT( "stat.log", false );
Cout CRUN( "run.log", false );
Cout CEXCEPT( "exception.log", false );

bool _need_daemon = false;

Cout::Cout( const string& file, bool debug )
: _file(file)
, _debug(debug)
{
}

Cout::~Cout()
{
}

Cout& Cout::operator<<( bool v )
{
	if ( canOutput() )
	{
		log( v? "true" : "false" );
	}
	return *this;
}

Cout& Cout::operator<<( char v )
{
	if ( canOutput() )
	{
		char szBuf[4] = {0};
		sprintf( szBuf, "%c", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( BYTE v )
{
	if ( canOutput() )
	{
		char szBuf[4] = {0};
		sprintf( szBuf, "%d", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( short v )
{
	if ( canOutput() )
	{
		char szBuf[6] = {0};
		sprintf( szBuf, "%d", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( WORD v )
{
	if ( canOutput() )
	{
		char szBuf[6] = {0};
		sprintf( szBuf, "%d", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( int v )
{
	if ( canOutput() )
	{
		char szBuf[11] = {0};
		sprintf( szBuf, "%d", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( DWORD v )
{
	if ( canOutput() )
	{
		char szBuf[11] = {0};
		sprintf( szBuf, "%u", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( long v )
{
	if ( canOutput() )
	{
		char szBuf[21] = {0};
		sprintf( szBuf, "%ld", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( ULONG v )
{
	if ( canOutput() )
	{
		char szBuf[21] = {0};
		sprintf( szBuf, "%lu", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( float v )
{
	if ( canOutput() )
	{
		char szBuf[21] = {0};
		sprintf( szBuf, "%f", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( double v )
{
	if ( canOutput() )
	{
		char szBuf[21] = {0};
		sprintf( szBuf, "%lf", v );
		log( szBuf );
	}
	return *this;
}

Cout& Cout::operator<<( const char* v )
{
	if ( canOutput() )
	{
		log( v );
	}
	return *this;
}

Cout& Cout::operator<<( const string& v )
{
	if ( canOutput() )
	{
		log( v );
	}
	return *this;
}

Cout& Cout::operator<<( Endl& el )
{
	if ( canOutput() )
	{
		log( "\n" );
	}
	return *this;
}

bool Cout::canOutput()
{
	return ( !_debug || ( _debug && UrlManager::getInstance()->isDebug() ) );
}
void Cout::log( const string& data )
{
	if ( _need_daemon )
	{
		if ( _dir.empty() )
		{
			_dir = CUR_DIR;
			_file = _dir + "/" + _file;
		}
		FILE* fp = fopen( _file.data(), "a+" );
		if ( NULL != fp )
		{
			fprintf( fp, "%s", data.data() );
			fflush(fp);
			fclose(fp);
		}
	}
	else
	{
		cout<<data;
	}
}
