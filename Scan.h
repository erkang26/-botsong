// FileName:Scan.h
//
// Desc:
//
// Created by token.tong at 2019-04-16 17:19:51
#ifndef _SCAN_H_
#define _SCAN_H_
#include "Utils.h"
#include "sharelib.h"

class Url;
class UrlManager;

class Scan
{
public:
	Scan();
	~Scan();

	void start();
	void stop();
	bool isStopped();
	void waitForStopping();

private:
	static void* threadEntry( void* p );
	void thread();

	void downloadImg( UrlManager* mg, Url* url );
	void downloadWeb( UrlManager* mg, Url* url );


private:
	pthread_t _tid;
	bool _stop;
	bool _stopped;
};

#endif //_SCAN_H_
