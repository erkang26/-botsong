// FileName: Scan.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-16 17:19:51
#include "Scan.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "UrlManager.h"
#include "Stat.h"
#include "UrlDelegate.h"

extern string IMG_DIR;
extern string HTML_DIR;


Scan::Scan()
: _stop(true)
, _stopped(true)
{
}

Scan::~Scan()
{
}

void Scan::start()
{
	_stop = false;
	pthread_create( &_tid, NULL, threadEntry, this );
}

void* Scan::threadEntry( void* p )
{
	Scan* pThis = (Scan*)p;
	pThis->thread();

	return NULL;
}
void Scan::thread()
{
	_stopped = false;
	UrlManager* mg = UrlManager::getInstance();

	Url* url = NULL;
	while( !_stop )
	{
		if ( NULL != ( url = mg->popImg() ) )
		{
			STAT_INCR( DownloadingImg );
			downloadImg( mg, url );
			STAT_DECR( DownloadingImg );
			STAT_INCR( DownloadedImg );
			continue;
		}

		if ( NULL != ( url = mg->popUrl() ) )
		{
			STAT_INCR( DownloadingUrl );
			downloadWeb( mg, url );
			STAT_DECR( DownloadingUrl );
			STAT_INCR( DownloadedUrl );
			continue;
		}

		sleep(1);
	}

	_stopped = true;
}

void Scan::downloadImg( UrlManager* mg, Url* url )
{
	HttpRequest* rq = new HttpRequest;
	HttpResponse* res = rq->get( url->getUrl() );

	if ( NULL != res )
	{
		if ( 200 != res->getCode() )
		{
			COUT<<res->getCode()<<": "<<res->getRequest()->getOriginUrl()<<ENDL;
			if ( 301 == res->getCode() && res->hasHeader( "Location" ) )
			{
				Url* o = new Url(*url);
				o->setUrl( res->getHeader( "Location" ) );
				mg->addImg( o );
			}
		}
		else
		{
			UrlDelegate* ud = UrlDelegate::getInstance( url->getFlag() );
			string data = res->getBody();
			data = ud->parseHtml( url, data );
			ud->save( url, IMG_DIR, data );
		}
	}
	else if ( rq->isTimeout() )
	{
		mg->addImg(url, true);
	}

	delete rq;
}

void Scan::downloadWeb( UrlManager* mg, Url* url )
{
	HttpRequest* rq = new HttpRequest;
	HttpResponse* res = rq->get( url->getUrl() );


	if ( NULL != res )
	{
		if ( 200 != res->getCode() )
		{
			COUT<<res->getCode()<<": "<<res->getRequest()->getOriginUrl()<<ENDL;
			if ( 301 == res->getCode() && res->hasHeader( "Location" ) )
			{
				Url* o = new Url(*url);
				o->setUrl( res->getHeader( "Location" ) );
				mg->addUrl( o );
			}
		}
		else
		{
			string data = res->getBody();
			UrlDelegate* ud = UrlDelegate::getInstance( url->getFlag() );
			data = ud->parseHtml( url, data );
			ud->save( url, HTML_DIR, data );
		}
	}
	else if ( rq->isTimeout() )
	{
		mg->addUrl( url, true );
	}

	delete rq;
}

void Scan::stop()
{
	_stop = true;
}

bool Scan::isStopped()
{
	return _stopped;
}

void Scan::waitForStopping()
{
	pthread_join( _tid, NULL );
}
