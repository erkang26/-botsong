// FileName:Exception.h
//
// Desc:
//
// Created by token.tong at 2019-05-07 15:43:23
#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_
#include <signal.h>
#include <setjmp.h>
#include "TlsStack.h"

typedef void (*exception_log_func)(const string&);

extern TlsStack* _exception_stack;
extern TlsStack* _exception_call_stack;

extern exception_log_func exception_log;

void _exception_sig_handler( int sig );
void exception_init( exception_log_func func = NULL);
void exception_uninit();

#define __TRY\
	try\
	{\
		sigjmp_buf* _env = (sigjmp_buf*)malloc(sizeof(sigjmp_buf));\
		_exception_stack->push( (void*)_env );\
		if ( 0 == sigsetjmp( *_env, 1 ) ) \
		{
#define __CATCH\
			sigjmp_buf* tmpEnv = (sigjmp_buf*)_exception_stack->pop();\
			free(tmpEnv);\
		}\
		else\
		{\
			sigjmp_buf* tmpEnv = (sigjmp_buf*)_exception_stack->pop();\
			free(tmpEnv);\
			throw "error";\
		}\
	}\
	catch( ... )\
	{\
		char szBuf[1024] = {0};\
		sprintf( szBuf, "%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__ );\
		string* call = new string(szBuf);\
		_exception_call_stack->push(call);\
		throw "error";\
	}
#define __FINALLY\
			sigjmp_buf* tmpEnv = (sigjmp_buf*)_exception_stack->pop();\
			free(tmpEnv);\
		}\
		else\
		{\
			sigjmp_buf* tmpEnv = (sigjmp_buf*)_exception_stack->pop();\
			free(tmpEnv);\
			throw "error";\
		}\
	}\
	catch( ... )\
	{\
		char szBuf[1024] = {0};\
		sprintf( szBuf, "%s:%d[%s]", __FILE__, __LINE__, __FUNCTION__ );\
		string* call = new string(szBuf);\
		_exception_call_stack->push(call);\
		string* s = NULL;\
		if ( NULL != exception_log )\
		{\
			exception_log( "exception start" );\
		}\
		while( NULL != (s=(string*)_exception_call_stack->pop()) )\
		{\
			if ( NULL != exception_log )\
			{\
				exception_log(*s);\
			}\
			delete s;\
			s = NULL;\
		}\
		if ( NULL != exception_log )\
		{\
			exception_log( "exception end" );\
		}\
	}


#endif //_EXCEPTION_H_
