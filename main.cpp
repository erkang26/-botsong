#include <iostream>
#include "UrlManager.h"
#include "Scan.h"
#include "Stat.h"
#include "UrlDelegate.h"
#include "Url.h"
#include "Exception.h"
#if KOS_TARGET == KOS_TARGET_LINUX
#include <signal.h>
#endif
using namespace std;

string getDateTimeString();
void createDir();
void save();
void load();
void except_log( const string& data );

string dateDir = getDateTimeString();
string IMG_DIR = "img";
string HTML_DIR = "html";

int main( int argc, char** argv )
{
	if ( argc < 2 )
	{
		cout<<"usage: botsong url [-s] [url] [url] ..."<<endl;

		exception_init( except_log );

		__TRY

			char* p = NULL;
			
			__TRY
			*p = 1;
			__CATCH

		__FINALLY
	/*
		ifstream fin( "/Users/tongyifeng/tmp/z.html" );
		string s;
		string tmp;
		while( getline( fin, tmp ) )
		{
			s += tmp;
		}
		fin.close();
		Url* url = new Url();
		UrlDelegate* o = UrlDelegate::getInstance( UT_NT_BQG_CONTENT );
		string data = o->parseHtml( url, s );
		cout<<data<<endl;
		*/
		/*
		cout<<s<<endl;
		UrlManager::getInstance()->parseWebUrl( "http://www.baidu.com", s );
		UrlManager::getInstance()->print();
		*/
//		NovBase* nov = NovBase::getInstance( NT_BQG );
		/*
		string content = nov->parseContent( s );
		cout<<content<<endl;
		*/
/*
		map<string,string> mapUrl;
		nov->parseList( s, mapUrl );
		for( map<string,string>::iterator itr = mapUrl.begin();
				mapUrl.end() != itr; ++itr )
		{
			cout<<itr->second<<": "<<itr->first<<endl;
		}
*/
		return -1;
	}
	exception_init( except_log );
	signal(SIGPIPE, SIG_IGN);
	UrlManager* mg = UrlManager::getInstance();
	bool sameSrc = false;
	Url* url = NULL;
	bool loaded = false;
	for ( int i=1; i<argc; ++i )
	{
		if ( 0 == strcmp( argv[i], "-s" ) )
		{
			sameSrc = true;
		}
		else if ( 0 == strcmp( argv[i], "-d" ) )
		{
			mg->setDebug(true);
		}
		else if ( 0 == strcmp( argv[i], "-l" ) )
		{
			load();
			loaded = true;
		}
		else if ( 0 == strcmp( argv[i], "-bqg" ) )
		{
			url = new Url();
			url->setUrl( argv[i] );
			url->setFlag( UT_NT_BQG_BK );
		}
		else
		{
			url = new Url();
			url->setUrl( argv[i] );
			url->setFlag( UT_COMMON );
		}
	}
	if ( !loaded && NULL != url )
	{
		mg->addUrl( url );
	}

	if ( sameSrc && NULL != url )
	{
		mg->setSameSource( url->getUrl() );
	}

	createDir();

	int threadNum = 20;

	Stat::getInstance()->start();

	vector<Scan*> vScan;
	for ( int i=0; i<threadNum; ++i )
	{
		Scan* s = new Scan;
		s->start();
		vScan.push_back(s);
	}

	bool saving = false;
	for ( ; ; )
	{
		string input;
		cin>>input;
		if ( "exit" == input )
		{
			break;
		}
		else if ( "save" == input )
		{
			saving = true;
			break;
		}
	}

	cout<<"stopping.."<<endl;

	for ( size_t i=0; i<vScan.size(); ++i )
	{
		vScan[i]->stop();
	}
	for ( size_t i=0; i<vScan.size(); ++i )
	{
		vScan[i]->waitForStopping();
		delete vScan[i];
	}
	vScan.clear();

	if ( saving )
	{
		save();
	}

	Stat::getInstance()->stop();
	Stat::getInstance()->waitForStopping();

	exception_uninit();

	cout<<"stopped"<<endl;

	return 0;
}

void createDir()
{
	string rootDir = "./" + dateDir;
	mkdir( rootDir.data(), 0777 );

	IMG_DIR = rootDir + "/" + IMG_DIR;
	HTML_DIR = rootDir + "/" + HTML_DIR;
 
	mkdir( IMG_DIR.data(), 0777 );
	mkdir( HTML_DIR.data(), 0777 );
}

string getDateTimeString()
{
	time_t ts = time(NULL);
	tm t;
	localtime_r( &ts, &t );

	char szTime[100] = {0};
	sprintf( szTime, "%04d%02d%02d%02d%02d%02d",
			t.tm_year+1900,
			t.tm_mon+1,
			t.tm_mday,
			t.tm_hour,
			t.tm_min,
			t.tm_sec );

	return string( szTime );
}

void save()
{
	UrlManager::getInstance()->saveUrl( "url.txt" );
	UrlManager::getInstance()->saveImg( "img.txt" );
}

void load()
{
	UrlManager::getInstance()->loadUrl( "url.txt" );
	UrlManager::getInstance()->loadImg( "img.txt" );
}
void except_log( const string& data )
{
	char szFileName[1024] = {0};
	sprintf( szFileName, "exception_%lu.log", (unsigned long)pthread_self() );
	FILE* fp = fopen( szFileName, "a+" );
	if ( NULL != fp )
	{
		fprintf( fp, "%s\n", data.data() );
		fflush(fp);
		fclose(fp);
	}
}
