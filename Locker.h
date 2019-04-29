// FileName:Locker.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 13:57:36
#ifndef _LOCKER_H_
#define _LOCKER_H_
#include "Utils.h"
#include "sharelib.h"

class Locker
{
public:
	Locker();
	~Locker();

	void lock();
	void unlock();

private:
	pthread_mutex_t _mutex;
};

class AutoLock
{
public:
	AutoLock( Locker& locker ) : _locker(&locker)
	{
		_locker->lock();
	}
	~AutoLock()
	{
		_locker->unlock();
	}

private:
	Locker* _locker;
};

#define StackLock( __locker__ ) AutoLock __sl(__locker__)
#define StackLockEx( __locker__, __sl__ ) AutoLock __sl__(__locker__)

#endif //_LOCKER_H_
