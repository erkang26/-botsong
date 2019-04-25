// FileName: Counter.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-17 09:55:51
#include "Counter.h"

Counter::Counter()
: _count(0)
{
	pthread_mutex_init( &_locker, NULL );
}

Counter::~Counter()
{
	pthread_mutex_destroy( &_locker );
}
void Counter::incr()
{
	pthread_mutex_lock( &_locker );
	++_count;
	pthread_mutex_unlock( &_locker );
}
void Counter::decr()
{
	pthread_mutex_lock( &_locker );
	--_count;
	pthread_mutex_unlock( &_locker );
}
void Counter::set( long count )
{
	pthread_mutex_lock( &_locker );
	_count = count;
	pthread_mutex_unlock( &_locker );
}
