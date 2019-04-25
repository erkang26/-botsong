// FileName: Stat.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-17 09:53:44
#include "Stat.h"

Stat::Stat()
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
	pthread_t tid;
	pthread_create( &tid, NULL, threadEntry, this );
}

void* Stat::threadEntry( void* p )
{
	Stat* pThis = (Stat*)p;
	pThis->thread();

	return NULL;
}

void Stat::thread()
{
	for ( ; ; )
	{
		cout<<endl;
		cout<<"waiting url: "<<_waitingUrlCount.get()<<endl;
		cout<<"waiting img: "<<_waitingImgCount.get()<<endl;
		cout<<"downloading url: "<<_downloadingUrlCount.get()<<endl;
		cout<<"downloading img: "<<_downloadingImgCount.get()<<endl;
		cout<<"downloaded url: "<<_downloadedUrlCount.get()<<endl;
		cout<<"downloaded img: "<<_downloadedImgCount.get()<<endl;
		cout<<endl;
		sleep(10);
	}
}
