// FileName: Cout.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-17 14:18:19
#include "cout.h"
#include "UrlManager.h"

Cout COUT;
Endl ENDL;

Cout::Cout()
{
}

Cout::~Cout()
{
}

Cout& Cout::operator<<( bool v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( char v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( BYTE v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( short v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( WORD v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( int v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( DWORD v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( long v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( ULONG v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( float v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( double v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( const char* v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( const string& v )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<v;
	}
	return *this;
}

Cout& Cout::operator<<( Endl& el )
{
	if ( UrlManager::getInstance()->isDebug() )
	{
		cout<<endl;
	}
	return *this;
}
