#include <iostream>
#include "UrlManager.h"
#include "Scan.h"
#include "Stat.h"
using namespace std;

string getDateTimeString();
void createDir();

string dateDir = getDateTimeString();
string IMG_DIR = "img";
string HTML_DIR = "html";

int main( int argc, char** argv )
{
	if ( argc < 2 )
	{
		cout<<"usage: botsong url [-s] [url] [url] ..."<<endl;
		ifstream fin( "/Users/tongyifeng/workbench/botsong/master/20190425160918/index.html" );
		string s;
		string tmp;
		while( getline( fin, tmp ) )
		{
			s += tmp;
		}
		fin.close();
		cout<<s<<endl;
		//string s = "<a href=/a/b.html>xxx</a><a href=/b/c.html x=d>dfds</a>";
		UrlManager::getInstance()->parseWebUrl( "http://www.baidu.com", s );
		UrlManager::getInstance()->print();
		return -1;
	}
	signal(SIGPIPE, SIG_IGN);
	UrlManager* mg = UrlManager::getInstance();
	bool sameSrc = false;
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
		else
		{
			mg->addUrl( argv[i] );
		}
	}

	if ( sameSrc )
	{
		for ( int i=1; i<argc; ++i )
		{
			if ( 0 != strcmp( argv[i], "-s" ) )
			{
				mg->setSameSource( argv[i] );
				break;
			}
		}
	}

	createDir();

	int threadNum = 10;

	Stat::getInstance()->start();

	vector<Scan*> vScan;
	for ( int i=0; i<threadNum; ++i )
	{
		Scan* s = new Scan;
		s->start();
		vScan.push_back(s);
	}

	for ( ; ; )
	{
		string input;
		cin>>input;
		if ( "exit" == input )
		{
			break;
		}
	}

	cout<<"stopping.."<<endl;

	Stat::getInstance()->stop();
	for ( size_t i=0; i<vScan.size(); ++i )
	{
		vScan[i]->stop();
	}
	Stat::getInstance()->waitForStopping();
	for ( size_t i=0; i<vScan.size(); ++i )
	{
		vScan[i]->waitForStopping();
		delete vScan[i];
	}
	vScan.clear();

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
