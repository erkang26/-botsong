// FileName: UrlManager.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-16 16:07:18
#include "UrlManager.h"
#include "HttpRequest.h"
#include "Stat.h"
#include "cout.h"

UrlManager::UrlManager()
: _sn(0)
, _debug(false)
{
}

UrlManager::~UrlManager()
{
}

static UrlManager* manager = NULL;
UrlManager* UrlManager::getInstance()
{
	if ( NULL != manager )
	{
		return manager; 
	}

	manager = new UrlManager;
	manager->init();

	return manager;
}

void UrlManager::init()
{
}

Url* UrlManager::popUrl()
{
	if ( _urlList.empty() )
	{
		return NULL;
	}

	Url* url = (Url*)_urlList.pop();
	if ( NULL == url )
	{
		return NULL;
	}

	STAT_DECR( WaitingUrl );
	
	return url;
}

Url* UrlManager::popImg()
{
	if ( _imgList.empty() )
	{
		return NULL;
	}

	Url* url = (Url*)_imgList.pop();
	if ( NULL == url )
	{
		return NULL;
	}

	STAT_DECR( WaitingImg );

	return url;
}

void UrlManager::addUrl( Url* url, bool force )
{
	if ( !_sameSrc.empty() )
	{
		if ( string::npos == url->getUrl().find( _sameSrc ) )
		{
			return ;
		}
	}

	if ( !force )
	{
		if ( !_mapUsedUrl.setnx( url->getUrl(), url ) )
		{
			return ;
		}
	}
	else
	{
		_mapUsedUrl.set( url->getUrl(), url );
	}

	_urlList.push(url);

	STAT_INCR( WaitingUrl );
}

void UrlManager::addImg( Url* url, bool force )
{
	if ( !force )
	{
		if ( !_mapUsedUrl.setnx( url->getUrl(), url ) )
		{
			return ;
		}
	}
	else
	{
		_mapUsedUrl.set( url->getUrl(), url );
	}

	_imgList.push(url);

	STAT_INCR( WaitingImg );
}

long UrlManager::getNextSn()
{
	StackLock( _snLocker );
	long r = ++_sn;

	return r;
}

void UrlManager::print()
{
	CRUN<<"url:"<<ENDL;
	list<void*> urlList;
	_urlList.copy(urlList);
	for ( list<void*>::iterator itr = urlList.begin(); urlList.end() != itr; ++itr )
	{
		Url* url = (Url*)(*itr);
		CRUN<<url->getUrl()<<ENDL;
	}

	CRUN<<"img:"<<ENDL;
	list<void*> imgList;
	_imgList.copy(imgList);
	for ( list<void*>::iterator itr = imgList.begin(); imgList.end() != itr; ++itr )
	{
		Url* url = (Url*)(*itr);
		CRUN<<url->getUrl()<<ENDL;
	}
}
void UrlManager::setSameSource( const string& url )
{
	int len = HTTP_HEADER_SIZE;
	size_t pos = url.find( HTTP_HEADER );
	if ( string::npos == pos )
	{
		len = HTTPS_HEADER_SIZE;
		pos = url.find( HTTPS_HEADER );
	}
	string tmp;
	if ( string::npos != pos )
	{
		tmp = url.substr( len, url.size()-len );
	}
	else
	{
		tmp = url;
	}

	pos = tmp.find( "/" );
	if ( string::npos != pos )
	{
		tmp = tmp.substr( 0, pos );
	}

	vector<string> v = Utils::split( tmp, "." );
	if ( 2 == v.size() )
	{
		_sameSrc =  tmp;
	}
	else
	{
		for ( size_t i=1; i<v.size(); ++i )
		{
			if ( i > 1 )
			{
				_sameSrc += ".";
			}
			_sameSrc +=  v[i];
		}
	}

	CRUN<<"sameSrc: "<<_sameSrc<<ENDL;
}

void UrlManager::loadUrl( const string& path )
{
	ifstream fin(path.data());
	if ( !fin.is_open() )
	{
		CERR<<"open file:"<<path<<" failed"<<ENDL;
		return ;
	}

	while( !fin.eof() )
	{
		char szLine[1024] = {0};
		fin.getline( szLine, 1024 );
		string line = szLine;

		Url* url = new Url();
		if ( url->load(line) )
		{
			addUrl(url);
		}
		else
		{
			CERR<<"load url:"<<" failed"<<ENDL;
			delete url;
			url = NULL;
		}
	}

	fin.close();
}
void UrlManager::loadImg( const string& path )
{
	ifstream fin(path.data());
	if ( !fin.is_open() )
	{
		CERR<<"open file:"<<path<<" failed"<<ENDL;
		return ;
	}

	while( !fin.eof() )
	{
		char szLine[1024] = {0};
		fin.getline( szLine, 1024 );
		string line = szLine;

		Url* url = new Url();
		if ( url->load(line) )
		{
			addImg(url);
		}
		else
		{
			CERR<<"load img:"<<line<<" failed"<<ENDL;
			delete url;
			url = NULL;
		}
	}

	fin.close();
}
bool UrlManager::saveUrl( const string& path )
{
	ofstream fout(path.data());
	if ( !fout.is_open() )
	{
		return false;
	}
	vector<void*> v = _urlList.popAll();
	for ( size_t i=0; i<v.size(); ++i )
	{
		Url* url = (Url*)v[i];
		fout<<url->serial()<<endl;
	}
	fout.close();

	return true;
}
bool UrlManager::saveImg( const string& path )
{
	ofstream fout(path.data());
	if ( !fout.is_open() )
	{
		return false;
	}
	vector<void*> v = _imgList.popAll();
	for ( size_t i=0; i<v.size(); ++i )
	{
		Url* url = (Url*)v[i];
		fout<<url->serial()<<endl;
	}
	fout.close();

	return true;
}
