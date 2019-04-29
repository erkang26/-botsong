// FileName: UrlManager.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-16 16:07:18
#include "UrlManager.h"
#include "HttpRequest.h"
#include "Stat.h"

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

void UrlManager::addUrl( Url* url )
{
	if ( !_sameSrc.empty() )
	{
		if ( string::npos == url->getUrl().find( _sameSrc ) )
		{
			return ;
		}
	}

	if ( !_mapUsedUrl.setnx( url->getUrl(), url ) )
	{
		return ;
	}

	_urlList.push(url);

	STAT_INCR( WaitingUrl );
}

void UrlManager::addImg( Url* url )
{
	if ( !_mapUsedUrl.setnx( url->getUrl(), url ) )
	{
		return ;
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
	cout<<"url:"<<endl;
	list<void*> urlList;
	_urlList.copy(urlList);
	for ( list<void*>::iterator itr = urlList.begin(); urlList.end() != itr; ++itr )
	{
		Url* url = (Url*)(*itr);
		cout<<url->getUrl()<<endl;
	}

	cout<<"img:"<<endl;
	list<void*> imgList;
	_imgList.copy(imgList);
	for ( list<void*>::iterator itr = imgList.begin(); imgList.end() != itr; ++itr )
	{
		Url* url = (Url*)(*itr);
		cout<<url->getUrl()<<endl;
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

	cout<<"sameSrc: "<<_sameSrc<<endl;
}
