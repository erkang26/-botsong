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

extern string IMG_DIR;
extern string HTML_DIR;

#define MIN_IMG_SIZE 10240

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

	string url;
	while( !_stop )
	{
		if ( mg->popImg( url ) )
		{
			STAT_INCR( DownloadingImg );
			downloadImg( mg, url );
			STAT_DECR( DownloadingImg );
			STAT_INCR( DownloadedImg );
			continue;
		}

		if ( mg->popUrl( url ) )
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

void Scan::downloadImg( UrlManager* mg, const string& url )
{
	HttpRequest* rq = new HttpRequest;
	HttpResponse* res = rq->get( url );

	if ( NULL != res )
	{
		if ( 200 != res->getCode() )
		{
			COUT<<res->getCode()<<": "<<res->getRequest()->getOriginUrl()<<ENDL;
			if ( 301 == res->getCode() && res->hasHeader( "Location" ) )
			{
				mg->addImg( res->getHeader( "Location" ) );
			}
		}
		else
		{
			const string& data = res->getBody();
			if ( data.size() >= MIN_IMG_SIZE )
			{
				string filePath = IMG_DIR + "/" + getImageName( url );
				ofstream fout( filePath.data(), ios::binary );
				if ( fout.is_open() )
				{
					fout.write( data.data(), data.size() );
					fout.close();
				}
			}
		}
	}

	delete rq;
}

void Scan::downloadWeb( UrlManager* mg, const string& url )
{
	HttpRequest* rq = new HttpRequest;
	HttpResponse* res = rq->get( url );


	if ( NULL != res )
	{
		if ( 200 != res->getCode() )
		{
			COUT<<res->getCode()<<": "<<res->getRequest()->getOriginUrl()<<ENDL;
			if ( 301 == res->getCode() && res->hasHeader( "Location" ) )
			{
				mg->addUrl( res->getHeader( "Location" ) );
			}
		}
		else
		{
			string htmlFile = getFileByUrl( HTML_DIR, url );
			htmlFile += ".html";
			ofstream fout( htmlFile.data() );
			if ( fout.is_open() )
			{
				fout<<res->getBody();
				fout.close();
			}
			mg->parseWebUrl( url, res->getBody() );
			mg->parseImgUrl( url, res->getBody() );
		}
	}
	delete rq;
}

string Scan::getFileByUrl( const string& rootDir, const string& url )
{
	string tmp = url;
	if ( 0 == tmp.find( HTTP_HEADER ) )
	{
		tmp.erase( 0, HTTP_HEADER_SIZE );
	}

	Utils::trimAll( tmp );

	string file = rootDir + "/" + tmp;

	return file;
}
string Scan::getImageName( string url )
{
	size_t n = url.rfind( '.' );
	string suffix;
	if ( string::npos != n )
	{
		suffix = url.substr( n, url.size()-n );
		if ( suffix.size() > 6 )
		{
			suffix.clear();
		}
	}

	char szTemp[50] = {0};
	sprintf( szTemp, "%09ld", UrlManager::getInstance()->getNextSn() );
	string file = szTemp;
	if ( suffix.empty() )
	{
		suffix = ".jpg";
	}
	file += suffix;
	return file;
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
