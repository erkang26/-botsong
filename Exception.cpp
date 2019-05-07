// FileName: Exception.cpp
//
// Desc:
//
// Created by token.tong at 2019-05-07 15:43:23
#include "Exception.h"

TlsStack* _exception_stack = NULL;
TlsStack* _exception_call_stack = NULL;

void _exception_sig_handler( int sig )
{
	sigjmp_buf* env = (sigjmp_buf*)_exception_stack->pop();
	if ( NULL != env )
	{
		siglongjmp( *env, 1 );
	}
}
void exception_init()
{
	_exception_stack = new TlsStack();
	_exception_call_stack = new TlsStack();

	struct sigaction act;
	act.sa_handler = _exception_sig_handler;
	sigemptyset( &act.sa_mask );
	act.sa_flags = 0;

	sigaction( SIGSEGV, &act, 0 );
}
void exception_uninit()
{
	if ( NULL != _exception_stack )
	{
		delete _exception_stack;
		_exception_stack = NULL;
	}
}
