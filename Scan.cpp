// FileName: Scan.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-16 17:19:51
#include "Scan.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "UrlManager.h"

extern string IMG_DIR;
extern string HTML_DIR;

Scan::Scan()
{
}

Scan::~Scan()
{
}

void Scan::start()
{
	pthread_t tid;
	pthread_create( &tid, NULL, threadEntry, this );
}

void* Scan::threadEntry( void* p )
{
	Scan* pThis = (Scan*)p;
	pThis->thread();

	return NULL;
}
void Scan::thread()
{
	UrlManager* mg = UrlManager::getInstance();

	string url;
	while( true )
	{
		if ( mg->popImg( url ) )
		{
			cout<<"downloading image.."<<endl;
			downloadImg( url );
			cout<<"downloaded image"<<endl;
			continue;
		}

		if ( mg->popUrl( url ) )
		{
			cout<<"downloading web.."<<endl;
			downloadWeb( mg, url );
			cout<<"downloaded web"<<endl;
			continue;
		}

		sleep(1);
	}
}

void Scan::downloadImg( const string& url )
{
	HttpRequest* rq = new HttpRequest;
	HttpResponse* res = rq->get( url );

	const string& data = res->getBody();
	if ( data.size() > 1024 )
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

void Scan::downloadWeb( UrlManager* mg, const string& url )
{
	HttpRequest* rq = new HttpRequest;
	HttpResponse* res = rq->get( url );

	string htmlFile = getFileByUrl( HTML_DIR, url );
	htmlFile += ".html";
	ofstream fout( htmlFile.data() );
	if ( fout.is_open() )
	{
		fout<<res->getBody();
		fout.close();
	}

	if ( NULL != res )
	{
		mg->parseWebUrl( url, res->getBody() );
		mg->parseImgUrl( url, res->getBody() );
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
	file += suffix;
	return file;
}
