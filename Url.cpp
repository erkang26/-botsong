// FileName: Url.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 14:23:46
#include "Url.h"

const char* SEPERATE_S = "__|,|__";
const char* VER = "1.0";

Url::Url()
: _flag(UT_NONE)
{
}

Url::Url( const Url& obj )
{
	_url = obj._url;
	_flag = obj._flag;
	_ex = obj._ex;
	_arr = obj._arr;
}

Url::~Url()
{
}

void Url::setUrl( const string& parentUrl, const string& url )
{
	string domain = getDomain( parentUrl );
	string domainWithDir = getDomainWithDir( parentUrl );

	_url = formatUrl( domain, domainWithDir, url );

}

string Url::getDomain( const string& url )
{
	int len = HTTP_HEADER_SIZE;
	size_t pos = url.find( HTTP_HEADER );
	if ( string::npos == pos )
	{
		len = HTTPS_HEADER_SIZE;
		pos = url.find( HTTPS_HEADER );
	}
	
	string ret;
	size_t start = 0;
	if ( string::npos != pos )
	{
		start = len;
	}
	else
	{
		ret = HTTP_HEADER;
	}

	pos = url.find( "/", start );
	if ( string::npos != pos )
	{
		ret += url.substr( 0, pos );
	}
	else
	{
		ret += url;
	}

	return ret;
}

string Url::getDomainWithDir( const string& url )
{
	int len = HTTP_HEADER_SIZE;
	size_t pos = url.find( HTTP_HEADER );
	if ( string::npos == pos )
	{
		len = HTTPS_HEADER_SIZE;
		pos = url.find( HTTPS_HEADER );
	}
	
	string ret;
	size_t start = 0;
	if ( string::npos != pos )
	{
		start = len;
	}
	else
	{
		ret = HTTP_HEADER;
	}

	pos = url.rfind( '/' );
	if ( string::npos != pos )
	{
		ret += url.substr( 0, pos );
	}
	else
	{
		ret += url;
	}

	return ret;
}

string Url::formatUrl( const string& domain, const string& domainWithDir, const string& url )
{
	string ret = url;
	size_t pos = ret.find( HTTP_HEADER );
	if ( string::npos == pos )
	{
		pos = ret.find( HTTPS_HEADER );
	}
	if ( string::npos == pos )
	{
		if ( '/' == ret[0] )
		{
			if ( ret.size() < 2 || '/' != ret[1] )
			{
				ret = domain + ret;
			}
			else
			{
				ret = "http:" + ret;
			}
		}
		else
		{
			ret = domainWithDir + "/" + ret;
		}
	}

	return ret;
}

void Url::reset()
{
	_url.clear();
	_flag = UT_NONE;
	_ex.clear();
	_arr.clear();
}

string Url::serial()
{
	string ret = VER;
	ret += SEPERATE_S;
	ret += _url;
	ret += SEPERATE_S;

	{
		char buf[50] = {0};
		sprintf( buf, "%d", _flag );
		ret += buf;
		ret += SEPERATE_S;
	}

	ret += _ex;
	for ( size_t i=0; i<_arr.size(); ++i )
	{
		ret += SEPERATE_S;
		ret += _arr[i];
	}

	return ret;
}
bool Url::load( const string& data )
{
	vector<string> v = Utils::split( data, SEPERATE_S );
	if ( v.empty() )
	{
		return false;
	}

	if ( "1.0" == v[0] )
	{
		return load1_0(v);
	}

	return false;
}

bool Url::load1_0( const vector<string>& v )
{
	if ( v.size() < 4 )
	{
		return false;
	}

	_url = v[1];
	_flag = UT_NONE;
	if ( !v[2].empty() )
	{
		_flag = (URL_TYPE)atoi(v[2].data());
	}
	if ( UT_NONE == _flag )
	{
		return false;
	}
	_ex = v[3];
	for ( size_t i=4; i<v.size(); ++i )
	{
		_arr.push_back(v[i]);
	}

	return true;
}
