// FileName: SocketClient.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-15 10:08:39
#include "SocketClient.h"
#include "X509Cert.h"
#include "cout.h"

const int RECV_TIME_OUT = 60;

SocketClient::SocketClient()
: _port(0)
, _ssl(false)
, _sock(INVALID_SOCK)
, _ctx(NULL)
, _sslHandle(NULL)
, _buf(NULL)
, _bufLen(0)
, _dataLen(0)
, _lastTimeout(false)
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
			COUT<<"gethostbyname error"<<ENDL;
			break;
		}

		_sock = socket( AF_INET, SOCK_STREAM, 0 );
		if ( INVALID_SOCK == _sock )
		{
			COUT<<"socket error"<<ENDL;
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
			COUT<<"connect error"<<ENDL;
			break;
		}

		timeval tv = { RECV_TIME_OUT, 0 };
		setsockopt( _sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&tv, sizeof(tv) );

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
			COUT<<"ctx new failed"<<ENDL;
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
			COUT<<ERR_error_string(e, NULL);
			COUT<<"ssl connect failed"<<ENDL;
			break;
		}

		X509* c = SSL_get_peer_certificate( _sslHandle );
		if ( NULL == c )
		{
			COUT<<"no cert"<<ENDL;
			break;
		}

		X509Cert* cert = X509Cert::parse( c );
		if ( NULL == cert )
		{
			COUT<<"parse cert failed"<<ENDL;
			break;
		}

		//cert->print();

		return true;
	} while(0);

	ERR_print_errors_fp(stdout);

	close();

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
			COUT<<"send failed"<<ENDL;
			return false;
		}

		sending -= ret;
		sent += ret;
	}

	COUT<<"send success"<<ENDL;
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
		_ctx = NULL;
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
	_lastTimeout = false;
	if ( _ssl )
	{
		int ret = SSL_read( _sslHandle, (char*)data, len );
		//COUT<<ret<<ENDL;
		if ( 0 != ret )
		{
			return ret;
		}
		else
		{
			int err = errno;
			if ( SSL_ERROR_ZERO_RETURN == SSL_get_error( _sslHandle, ret ) )
			{
				return 0;
			}
			else
			{
				COUT<<SSL_get_error( _sslHandle, ret )<<ENDL;
				if ( EAGAIN == err )
				{
					_lastTimeout = true;
				}
				return -1;
			}
		}
	}
	else
	{
		int r = ::recv( _sock, (char*)data, len, 0 );
		if ( EAGAIN == errno )
		{
			_lastTimeout = true;
		}
		COUT<<errno<<ENDL;
		return r;
	}
}
