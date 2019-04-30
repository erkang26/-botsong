// FileName:TsList.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 14:43:50
#ifndef _TS_LIST_H_
#define _TS_LIST_H_
#include "Utils.h"
#include "sharelib.h"
#include "Locker.h"

class TsList
{
public:
	TsList();
	~TsList();

	void push(void* p);
	void* pop();
	vector<void*> popAll();
	size_t size() const { return _list.size(); }
	bool empty() const { return _list.empty(); }

	void copy( list<void*>& lst ) { _list = lst; }

private:
	list<void*> _list;
	Locker _locker;
};

#endif //_TS_LIST_H_
