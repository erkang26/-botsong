// FileName: TlsStack.cpp
//
// Desc:
//
// Created by token.tong at 2019-05-07 15:23:24
#include "TlsStack.h"
#include "Stack.h"

static void __stack_free__( void* p )
{
	if ( NULL != p )
	{
		Stack* st = (Stack*)p;
		delete st;
	}
}

TlsStack::TlsStack()
{
	pthread_key_create( &_key, __stack_free__ );
}

TlsStack::~TlsStack()
{
	pthread_key_delete( _key );
}

void TlsStack::push( void* p )
{
	lazy_init();
	Stack* st = (Stack*)pthread_getspecific( _key );
	st->push(p);
}
void* TlsStack::pop()
{
	lazy_init();
	Stack* st = (Stack*)pthread_getspecific( _key );
	return st->pop();
}
void* TlsStack::top()
{
	lazy_init();
	Stack* st = (Stack*)pthread_getspecific( _key );
	return st->top();
}
bool TlsStack::empty()
{
	lazy_init();
	Stack* st = (Stack*)pthread_getspecific( _key );
	return st->empty();
}
size_t TlsStack::size()
{
	lazy_init();
	Stack* st = (Stack*)pthread_getspecific( _key );
	return st->size();
}

void TlsStack::lazy_init()
{
	void* p = pthread_getspecific( _key );
	if ( NULL == p )
	{
		Stack* st = new Stack();
		pthread_setspecific( _key, st );
	}
}
