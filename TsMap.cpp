// FileName: TsMap.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 14:10:20
#include "TsMap.h"

TsMap::TsMap()
{
}

TsMap::~TsMap()
{
}

bool TsMap::setnx( const string& key, void* value )
{
	StackLock( _locker );
	if ( _mapData.end() != _mapData.find(key) )
	{
		return false;
	}

	_mapData[key] = value;

	return true;
}
void TsMap::set( const string& key, void* value )
{
	StackLock(_locker);
	_mapData[key] = value;
}
void* TsMap::get( const string& key )
{
	StackLock(_locker);
	map<string,void*>::iterator itr = _mapData.find(key);
	if ( _mapData.end() != itr )
	{
		return itr->second;
	}

	return NULL;
}
