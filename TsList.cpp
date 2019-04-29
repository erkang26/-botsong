// FileName: TsList.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 14:43:50
#include "TsList.h"

TsList::TsList()
{
}

TsList::~TsList()
{
}

void TsList::push(void* p)
{
	StackLock(_locker);
	_list.push_back(p);
}
void* TsList::pop()
{
	StackLock(_locker);
	if ( _list.empty() )
	{
		return NULL;
	}
	void* p = _list.front();
	_list.pop_front();

	return p;
}
