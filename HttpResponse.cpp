// FileName: HttpResponse.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-15 15:57:46
#include "HttpResponse.h"
#include "Cookie.h"
#include "cout.h"

HttpResponse::HttpResponse( HttpRequest* request )
: _request(request)
{
}

HttpResponse::~HttpResponse()
{
}

bool HttpResponse::parse( const string& data )
{
	size_t pos = data.find( "\r\n\r\n" );
	if ( string::npos == pos )
	{
		return false;
	}
	string header = data.substr( 0, pos+2 );
	parseHeader( header );

	string body = data.substr( pos+4, data.size()-pos-4 );
	parseBody( body );

	return true;
}

bool HttpResponse::parseHeader( const string& header )
{
	size_t pos = header.find( "\r\n" );
	string line = header.substr( 0, pos );
	pos += 2;
	parseFirstLine( line );

	for ( ; ; )
	{
		size_t pos2 = header.find( "\r\n", pos );
		if ( string::npos == pos2 )
		{
			break;
		}
		line = header.substr( pos, pos2-pos+1 );
		pos = pos2+2;

		size_t n = line.find( ": " );
		string key = line.substr( 0, n );
		string value = line.substr( n+2, line.size()-n-2 );
		if ( "Set-Cookie" == key )
		{
			Cookie* ck = new Cookie;
			if ( !ck->parse( value ) )
			{
				COUT<<"parse cookie failed:"<<value<<ENDL;
				delete ck;
			}
			else
			{
				_cookies.push_back(ck);
			}
		}
		else
		{
			_headers[key] = value;
		}
	}

	return true;
}

bool HttpResponse::parseBody( const string& body )
{
	if ( hasHeader( "Transfer-Encoding" ) )
	{
		size_t n = 0;
		size_t m = 0;
		for( ; ; )
		{
			m = body.find( "\r\n", n );
			if ( string::npos == m )
			{
				break;
			}

			string s = body.substr( n, m-n );
			long l = strtol( s.data(), NULL, 16 );
			if ( 0 == l )
			{
				break;
			}
			
			if ( (m+2+l) > body.size() )
			{
				l = body.size() - m - 2;
			}
			_body.append( (char*)body.data()+m+2, l );
			n = m+2+l+2;
		}
	}
	else
	{
		_body = body;
	}

	COUT<<_body<<ENDL;
	COUT<<ENDL<<ENDL<<ENDL;
	COUT<<"__start__"<<ENDL;
	COUT<<body<<ENDL;
	
	return true;
}

void HttpResponse::parseFirstLine( const string& line )
{
	size_t n = line.find( " " );
	_ver = line.substr( 0, n );
	size_t m = line.find( " ", n+1 );
	_code = atoi(line.substr( n+1, m-n).data());
	_codeDesc = line.substr( m+1, line.size()-m-1 );

}

bool HttpResponse::hasHeader( const string& key )
{
	return ( _headers.end() != _headers.find(key) );
}
const string& HttpResponse::getHeader( const string& name )
{
	map< string, string >::iterator itr = _headers.find( name );
	if ( _headers.end() != itr )
	{
		return itr->second;
	}

	return _empty;
}
