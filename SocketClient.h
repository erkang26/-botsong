// FileName:SocketClient.h
//
// Desc:
//
// Created by token.tong at 2019-04-15 10:08:39
#ifndef _SOCKET_CLIENT_H_
#define _SOCKET_CLIENT_H_
#include "Utils.h"
#include "sharelib.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

class SocketClient
{
public:
	SocketClient();
	~SocketClient();

	bool connect( const char* pszHost, WORD port, bool ssl = false );

	bool send( void* data, int len );
	bool recv( string& data, int len );

	void close();

	int doSend( void* data, int len );
	int doRecv( void* data, int len );

	bool isLastTimeout() const { return _lastTimeout; }

private:
	bool connectNormal();
	bool connectSsl();

	void initBuf();
	void expandBuf();


	string _host;
	WORD _port;
	bool _ssl;

	SOCK_T _sock;

	SSL_CTX* _ctx;
	SSL* _sslHandle;

	char* _buf;
	int _bufLen;
	int _dataLen;

	bool _lastTimeout;
};

#endif //_SOCKET_CLIENT_H_
