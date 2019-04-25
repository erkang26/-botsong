// FileName:Stat.h
//
// Desc:
//
// Created by token.tong at 2019-04-17 09:53:44
#ifndef _STAT_H_
#define _STAT_H_
#include "Utils.h"
#include "sharelib.h"
#include "Counter.h"

#define STAT_INCR( _name_ ) Stat::getInstance()->get##_name_##Count().incr()
#define STAT_DECR( _name_ ) Stat::getInstance()->get##_name_##Count().decr()

class Stat
{
public:
	Stat();
	~Stat();

	static Stat* getInstance();

	void start();

	Counter& getWaitingUrlCount() { return _waitingUrlCount; }
	Counter& getWaitingImgCount() { return _waitingImgCount; }
	Counter& getDownloadingUrlCount() { return _downloadingUrlCount; }
	Counter& getDownloadingImgCount() { return _downloadingImgCount; }
	Counter& getDownloadedUrlCount() { return _downloadedUrlCount; }
	Counter& getDownloadedImgCount() { return _downloadedImgCount; }

private:
	static void* threadEntry( void * p );
	void thread();

private:
	Counter _waitingUrlCount;
	Counter _waitingImgCount;
	Counter _downloadingUrlCount;
	Counter _downloadingImgCount;
	Counter _downloadedUrlCount;
	Counter _downloadedImgCount;

};

#endif //_STAT_H_
