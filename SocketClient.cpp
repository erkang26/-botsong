// FileName: SocketClient.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-15 10:08:39
#include "SocketClient.h"
#include "X509Cert.h"

SocketClient::SocketClient()
: _port(0)
, _ssl(false)
, _sock(INVALID_SOCK)
, _ctx(NULL)
, _sslHandle(NULL)
, _buf(NULL)
, _bufLen(0)
, _dataLen(0)
{
}

SocketClient::~SocketClient()
{
	close();
}

bool SocketClient::connect( const char* pszHost, WORD port, bool ssl )
{
	if ( NULL != pszHost )
	{
		_host = pszHost;
	}
	else
	{
		_host = "127.0.0.1";
	}

	if ( 0 != port )
	{
		_port = port;
	}
	else
	{
		if ( ssl )
		{
			_port = 443;
		}
		else
		{
			_port = 80;
		}
	}

	_ssl = ssl;

	if ( _ssl )
	{
		return connectSsl();
	}
	else
	{
		return connectNormal();
	}
}

bool SocketClient::connectNormal()
{
#ifdef WIN32
	assert(false);
#else
	
	do
	{
		hostent* p = gethostbyname( _host.data() );
		if ( NULL == p )
		{
			cout<<"gethostbyname error"<<endl;
			break;
		}

		_sock = socket( AF_INET, SOCK_STREAM, 0 );
		if ( INVALID_SOCK == _sock )
		{
			cout<<"socket error"<<endl;
			break;
		}

		sockaddr_in addr;
		memset( &addr, 0, sizeof(addr) );
		addr.sin_family = AF_INET;
		addr.sin_port = htons(_port);

		memcpy( &addr.sin_addr, p->h_addr_list[0], p->h_length );

		int ret = ::connect( _sock, (sockaddr*)&addr, sizeof(addr) );
		if ( SOCK_ERROR == ret )
		{
			cout<<"connect error"<<endl;
			break;
		}

		return true;
	} while(0);

	if ( INVALID_SOCK != _sock )
	{
		::close(_sock);
		_sock = INVALID_SOCK;
	}

#endif

	return false;
}

bool SocketClient::connectSsl()
{
	do
	{
		SSL_library_init();
		OpenSSL_add_all_algorithms();
		SSL_load_error_strings();

		_ctx = SSL_CTX_new( SSLv23_client_method() );
		if ( NULL == _ctx )
		{
			cout<<"ctx new failed"<<endl;
			break;
		}

		_sslHandle = SSL_new( _ctx );

		if ( !connectNormal() )
		{
			break;
		}

		SSL_set_fd( _sslHandle, _sock );
		if ( -1 == SSL_connect( _sslHandle ) )
		{
			unsigned long e = ERR_get_error();
			cout<<ERR_error_string(e, NULL);
			cout<<"ssl connect failed"<<endl;
			break;
		}

		X509* c = SSL_get_peer_certificate( _sslHandle );
		if ( NULL == c )
		{
			cout<<"no cert"<<endl;
			break;
		}

		X509Cert* cert = X509Cert::parse( c );
		if ( NULL == cert )
		{
			cout<<"parse cert failed"<<endl;
			break;
		}

		//cert->print();

		return true;
	} while(0);

	ERR_print_errors_fp(stdout);

	if ( NULL != _sslHandle )
	{
		SSL_shutdown( _sslHandle );
		SSL_free( _sslHandle );
		_sslHandle = NULL;
	}

	if ( NULL != _ctx )
	{
		SSL_CTX_free( _ctx );
	}

	if ( INVALID_SOCK != _sock )
	{
		::close( _sock );
		_sock = INVALID_SOCK;
	}

	return false;
}

bool SocketClient::send( void* data, int len )
{
	int sending = len;
	int sent = 0;
	while( sending > 0 )
	{
		int ret = doSend( (char*)data, len );
		if ( ret <= 0 )
		{
			return false;
		}

		sending -= ret;
		sent += ret;
	}

	return true;
}
bool SocketClient::recv( string& data, int len )
{
	while( (_dataLen+len) > _bufLen )
	{
		expandBuf();
	}

	int receiving = len;
	int received = _dataLen;
	while( receiving > 0 )
	{
		int ret = doRecv( _buf+received, receiving );
		if ( ret < 0 )
		{
			return false;
		}
		else if ( 0 == ret )
		{
			break;
		}
		receiving -= ret;
		received += ret;
	}
	_dataLen += len;

	data.append( (char*)_buf, _dataLen );
	_dataLen = 0;

	return true;
}

void SocketClient::close()
{
	if ( NULL != _sslHandle )
	{
		SSL_shutdown( _sslHandle );
		SSL_free( _sslHandle );
		_sslHandle = NULL;
	}

	if ( NULL != _ctx )
	{
		SSL_CTX_free( _ctx );
	}

	if ( INVALID_SOCK != _sock )
	{
		::close( _sock );
		_sock = INVALID_SOCK;
	}

	if ( NULL != _buf )
	{
		delete [] _buf;
		_buf = NULL;
		_bufLen = 0;
		_dataLen = 0;
	}
}
void SocketClient::initBuf()
{
	if ( NULL != _buf )
	{
		delete[] _buf;
		_buf = NULL;
		_bufLen = 0;
	}

	_bufLen = 1024;
	_dataLen = 0;
	_buf = new char[_bufLen];
}
void SocketClient::expandBuf()
{
	char* p = new char[_bufLen*2];
	if ( _dataLen > 0 )
	{
		memcpy( p, _buf, _dataLen );
	}
	delete [] _buf;
	_buf = p;
	_bufLen *= 2;
}
int SocketClient::doSend( void* data, int len )
{
	if ( _ssl )
	{
		return SSL_write( _sslHandle, (char*)data, len );
	}
	else
	{
		return ::send( _sock, (char*)data, len, 0 );
	}
}
int SocketClient::doRecv( void* data, int len )
{
	if ( _ssl )
	{
		int ret = SSL_read( _sslHandle, (char*)data, len );
		//cout<<ret<<endl;
		if ( 0 != ret )
		{
			return ret;
		}
		else
		{
			if ( SSL_ERROR_ZERO_RETURN == SSL_get_error( _sslHandle, ret ) )
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
	}
	else
	{
		return ::recv( _sock, (char*)data, len, 0 );
	}
}
