// FileName:TsMap.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 14:10:20
#ifndef _TS_MAP_H_
#define _TS_MAP_H_
#include "Utils.h"
#include "sharelib.h"
#include "Locker.h"

class TsMap
{
public:
	TsMap();
	~TsMap();

	bool setnx( const string& key, void* value );
	void set( const string& key, void* value );
	void* get( const string& key );
	inline size_t size() { return _mapData.size(); }
	inline bool empty() { return _mapData.empty(); }

	void copy( map<string,void*>& mapData ) { mapData = _mapData; }

private:
	map<string, void*> _mapData;
	Locker _locker;
};

#endif //_TS_MAP_H_
