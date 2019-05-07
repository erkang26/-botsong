// FileName:TlsStack.h
//
// Desc:
//
// Created by token.tong at 2019-05-07 15:23:24
#ifndef _TLS_STACK_H_
#define _TLS_STACK_H_
#include "Utils.h"
#include "sharelib.h"

class TlsStack
{
public:
	TlsStack();
	~TlsStack();

	void push( void* p );
	void* pop();
	bool empty();
	size_t size();

private:
	void lazy_init();

private:
	pthread_key_t _key;
};

#endif //_TLS_STACK_H_
