// FileName: Stat.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-17 09:53:44
#include "Stat.h"
#include "cout.h"

Stat::Stat()
: _stop(true)
, _stopped(true)
{
}

Stat::~Stat()
{
}

static Stat* inst = NULL;
Stat* Stat::getInstance()
{
	if ( NULL != inst )
	{
		return inst;
	}

	inst = new Stat;

	return inst;
}

void Stat::start()
{
	_stop = false;
	pthread_create( &_tid, NULL, threadEntry, this );
}

void* Stat::threadEntry( void* p )
{
	Stat* pThis = (Stat*)p;
	pThis->thread();

	return NULL;
}

void Stat::thread()
{
	const int SLEEP = 1;
	_stopped = false;
	while( !_stop )
	{
		
		CSTAT<<ENDL;
		CSTAT<<"waiting url: "<<_waitingUrlCount.get()<<ENDL;
		CSTAT<<"waiting img: "<<_waitingImgCount.get()<<ENDL;
		CSTAT<<"downloading url: "<<_downloadingUrlCount.get()<<ENDL;
		CSTAT<<"downloading img: "<<_downloadingImgCount.get()<<ENDL;
		CSTAT<<"downloaded url: "<<_downloadedUrlCount.get()<<ENDL;
		CSTAT<<"downloaded img: "<<_downloadedImgCount.get()<<ENDL;
		CSTAT<<ENDL;
		
		sleep(SLEEP);
	}

	_stopped = true;
}
void Stat::stop()
{
	_stop = true;
}
bool Stat::isStopped()
{
	return _stopped;
}
void Stat::waitForStopping()
{
	pthread_join( _tid, NULL );
}
