// FileName: Cookie.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-15 17:54:08
#include "Cookie.h"

Cookie::Cookie()
: _secure(false)
, _maxAge(0)
, _httpOnly(false)
, _expiresTime(0)
{
}

Cookie::~Cookie()
{
}

bool Cookie::parse( const string& line )
{
	vector<string> arr = Utils::split( line, ";" );
	for ( size_t i=0; i<arr.size(); ++i )
	{
		string& tmp = Utils::trimHead( arr[i], NULL );
		Utils::trimTail( tmp, NULL );
		size_t pos = tmp.find( "=" );
		if ( string::npos != pos )
		{
			string key = tmp.substr( 0, pos );
			string value = tmp.substr( pos+1, tmp.size()-pos-1 );
			if ( 0 == Utils::cmpNoCase( "Expires", key ) )
			{
				_expires = value;
			}
			else if ( 0 == Utils::cmpNoCase( "Path", key ) )
			{
				_path = value;
			}
			else if ( 0 == Utils::cmpNoCase( "Domain", key ) )
			{
				_domain = value;
			}
			else if ( 0 == Utils::cmpNoCase( "Max-Age", key ) )
			{
				_maxAge = atol(value.data());
			}
			else if ( 0 == Utils::cmpNoCase( "HttpOnly", key ) )
			{
				_httpOnly = ( 0==Utils::cmpNoCase( "true", value ) );
			}
			else
			{
				_name = key;
				_value = value;
			}
		}
		else
		{
			if ( 0 == Utils::cmpNoCase( "secure", tmp ) )
			{
				_secure = true;
			}
		}
	}

	transTime();

	return ( !_name.empty() );
}

void Cookie::transTime()
{
	if ( getMaxAge() > 0 )
	{
		_expiresTime = time(NULL) + getMaxAge();
	}
	else if ( !_expires.empty() )
	{
		tm t;
		strptime( _expires.data(), "%a, %d %b %Y %H:%M:%S", &t );
		_expiresTime = mktime(&t);
	}
}

void Cookie::print() const
{
	cout<<_name<<"="<<_value;
	if ( !_expires.empty() )
	{
		cout<<"; Expires="<<_expires;
	}

	if ( !_path.empty() )
	{
		cout<<"; Path="<<_path;
	}

	if ( isSecure() )
	{
		cout<<"; SECURE";
	}

	if ( isHttpOnly() )
	{
		cout<<"; HttpOnly=true";
	}

	if ( getMaxAge() > 0 )
	{
		cout<<"; Max-Age="<<getMaxAge();
	}

	if ( !_domain.empty() )
	{
		cout<<"; Domain="<<_domain;
	}

	cout<<"expiresTime="<<getExpiresTime();
	cout<<endl;
}
