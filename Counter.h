// FileName:Counter.h
//
// Desc:
//
// Created by token.tong at 2019-04-17 09:55:51
#ifndef _COUNTER_H_
#define _COUNTER_H_
#include "Utils.h"
#include "sharelib.h"

class Counter
{
public:
	Counter();
	~Counter();

	void incr();
	void decr();
	void set( long count );

	long get() const { return _count; }
	const string& getString();

private:
	long _count;
	string _s;
	pthread_mutex_t _locker;
};

#endif //_COUNTER_H_
