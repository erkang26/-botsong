// FileName: Locker.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 13:57:36
#include "Locker.h"

Locker::Locker()
{
	pthread_mutex_init( &_mutex, NULL );
}

Locker::~Locker()
{
	pthread_mutex_destroy( &_mutex );
}

void Locker::lock()
{
	pthread_mutex_lock( &_mutex );
}
void Locker::unlock()
{
	pthread_mutex_unlock( &_mutex );
}
