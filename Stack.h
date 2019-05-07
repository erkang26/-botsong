// FileName:Stack.h
//
// Desc:
//
// Created by token.tong at 2019-05-07 15:31:37
#ifndef _STACK_H_
#define _STACK_H_
#include <stack>
#include "Utils.h"
#include "sharelib.h"

class Stack
{
public:
	Stack();
	~Stack();

	void push( void* p );
	void* pop();
	void* top();
	bool empty();
	size_t size();

private:
	stack<void*> _stack;
};

#endif //_STACK_H_
