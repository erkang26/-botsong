// FileName: HttpRequest.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-15 15:22:21
#include "HttpRequest.h"
#include "SocketClient.h"
#include "HttpResponse.h"
#include "cout.h"


HttpRequest::HttpRequest()
: _sock(NULL)
, _response(NULL)
, _port(0)
, _ssl(false)
, _timeout(false)
{
}

HttpRequest::~HttpRequest()
{
	if ( NULL != _response )
	{
		delete _response;
	}

	if ( NULL != _sock )
	{
		delete _sock; 
	}
}

HttpResponse* HttpRequest::get( const string& url )
{
	_originUrl = url;
	parseRequest( url );
	addHeader( "Host", _host );
	addHeader( "Accept", "*/*" );
	addHeader( "User-Agent", "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_14_0) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/75.0.3759.4 Safari/537.36" );
	addHeader( "Connection", "close" );
	if ( !doRequest( "GET" ) )
	{
		COUT<<"doRequest failed"<<ENDL;
		return NULL;
	}

	return _response;
}

void HttpRequest::addHeader( const string& key, const string& value )
{
	_headers[key] = value;
}
void HttpRequest::addHeader( const string& key, long value )
{
	char buf[50] = {0};
	sprintf( buf, "%ld", value );
	_headers[key] = buf;
}
void HttpRequest::addHeader( const string& key, int value )
{
	char buf[50] = {0};
	sprintf( buf, "%d", value );
	_headers[key] = buf;
}
void HttpRequest::addHeader( const string& key, float value )
{
	char buf[50] = {0};
	sprintf( buf, "%f", value );
	_headers[key] = buf;
}

void HttpRequest::makeRequest( string& data, const string& method )
{
	data = method;
	data += " ";
	data += _uri;
	data.append( " HTTP/1.1\r\n" );
	
	for( map<string,string>::iterator itr = _headers.begin(); _headers.end() != itr; ++itr )
	{
		data += itr->first;
		data += ": ";
		data += itr->second;
		data += "\r\n";
	}
	if ( !_body.empty() )
	{
		data += "Content-Length: ";
		char buf[20] = {0};
		sprintf( buf, "%ld", _body.size() );
		data += buf;
		data += "\r\n";
	}
	data += "\r\n";
	if ( !_body.empty() )
	{
		data += _body;
	}
}

void HttpRequest::parseRequest( const string& url )
{
	string tmp;
	_ssl = true;
	size_t pos = url.find( HTTPS_HEADER );
	if ( string::npos == pos )
	{
		_ssl = false;
		pos = url.find( HTTP_HEADER );
	}
	if ( string::npos != pos )
	{
		int len = 0;
		if ( _ssl )
		{
			len = strlen(HTTPS_HEADER);
		}
		else
		{
			len = strlen(HTTP_HEADER);
		}
		tmp = url.substr( pos+len, url.size()-len-pos );
	}
	else
	{
		_ssl = false;
		tmp = url;
	}

	pos = tmp.find( "/" );
	if ( string::npos == pos )
	{
		_uri = "/";
	}
	else
	{
		_uri = tmp.substr( pos, tmp.size()-pos );
		tmp = tmp.substr( 0, pos );
		Utils::trimTail( _uri );
	}

	pos = tmp.find( ":" );
	if ( string::npos == pos )
	{
		if ( _ssl )
		{
			_port = 443;
		}
		else
		{
			_port = 80;
		}
	}
	else
	{
		_port = atoi( tmp.substr( pos+1, tmp.size()-pos-1 ).data() );
		tmp = tmp.substr( 0, pos );
	}

	_host = tmp;
}
bool HttpRequest::doRequest( const string& method )
{
	do
	{
		string data;
		makeRequest( data, method );

		COUT<<data<<ENDL;

		_sock = new SocketClient;
		if ( !_sock->connect( _host.data(), _port, _ssl ) )
		{
			COUT<<"connect failed"<<ENDL;
			break;
		}

		if ( !_sock->send( (char*)data.data(), data.size() ) )
		{
			COUT<<"send failed"<<ENDL;
			break;
		}

		char buf[1024] = {0};
		string recvData;
		bool e = false;
		int contentLen = 0;
		int leftLen = 0;
		for ( ; ; )
		{
			int r = _sock->doRecv( buf, sizeof(buf) );
			if ( r < 0 )
			{
				if ( _sock->isLastTimeout() )
				{
					_timeout = true;
				}
				COUT<<"recv failed: "<<errno<<ENDL;
				e = true;
				break;
			}
			else if ( 0 == r )
			{
				COUT<<"remote closed"<<ENDL;
				break;
			}
			else
			{
				COUT<<buf;
				recvData.append( buf, r );
			}
			size_t pos = recvData.find( "\r\n\r\n" );
			if ( string::npos != pos )
			{
				string finder;
				if ( string::npos != recvData.find( "Transfer-Encoding: chunked\r\n" ) )
				{
					finder = "0\r\n\r\n";
				}
				else if ( string::npos != recvData.find( "Content-Length: " ) )
				{
					size_t tmpPos = recvData.find( "Content-Length: " );
					size_t tmpPos2 = recvData.find( "\r\n", tmpPos );
					if ( string::npos != tmpPos2 )
					{
						int l = tmpPos + strlen("Content-Length: " );
						string tmpLen = recvData.substr( l, tmpPos2-l );
						Utils::trimHead( Utils::trimTail( tmpLen ) );
						if ( !tmpLen.empty() )
						{
							contentLen = atoi(tmpLen.data());
							leftLen = contentLen;
						}
					}
				}
				else
				{
					finder = "\r\n\r\n";
				}
				if ( contentLen > 0 )
				{
					pos += 4;
					leftLen = contentLen - (recvData.size()-pos);
					if ( leftLen <= 0 )
					{
						break;
					}
				}
				else
				{
					pos = recvData.find( finder, pos+4 );
					if ( string::npos != pos )
					{
						//cout<<"pos2:"<<pos<<endl;
						break;
					}
				}
			}
		}
		//COUT<<recvData<<ENDL;
		if ( e )
		{
			break;
		}

		_response = new HttpResponse(this);
		if ( !_response->parse( recvData ) )
		{
			break;
		}

		return true;
	} while(0);

	if ( NULL != _response )
	{
		delete _response;
		_response = NULL;
	}

	if ( NULL != _sock )
	{
		delete _sock;
		_sock = NULL;
	}

	return false;
}
