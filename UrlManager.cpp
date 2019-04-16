// FileName: UrlManager.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-16 16:07:18
#include "UrlManager.h"
#include "HttpRequest.h"

UrlManager::UrlManager()
: _sn(0)
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
	pthread_mutex_init( &_mapLocker, NULL );
	pthread_mutex_init( &_urlLocker, NULL );
	pthread_mutex_init( &_imgLocker, NULL );
	pthread_mutex_init( &_snLocker, NULL );
}

void UrlManager::parseWebUrl( const string& domainUrl, const string& data )
{
	string domain = getDomain( domainUrl );
	string tag = "href=\"";
	size_t n = 0;
	size_t m = 0;
	n = data.find( tag, m );
	while( string::npos != n )
	{
		n += tag.size();
		size_t np = data.find( "\"", n );
		if ( string::npos != np )
		{
			string url = data.substr( n, np-n );
			m = np+1;
			formatUrl( domain, url );
			addUrl( url );
		}
		else
		{
			m = n;
		}

		n = data.find( tag, m );
	}
}

void UrlManager::parseImgUrl( const string& url, const string& data )
{
	string domain = getDomain( url );

	string tag = "<img ";
	string att1 = "src=\"";
	string att2 = "lazy-src=\"";
	size_t n = 0;
	size_t m = 0;
	size_t n0 = 0;
	n0 = data.find( tag, m );
	while( string::npos != n0 )
	{
		n0 += tag.size();
		size_t n2 = data.find( att2, n0 );
		if ( string::npos == n2 || n2 > data.find( ">", n0 ) )
		{
			n = data.find( att1, n0 );
			if ( string::npos == n )
			{
				m = n0;
				continue;
			}
			else
			{
				n += att1.size();
			}
		}
		else
		{
			n += n2 + att2.size();
		}

		size_t np = data.find( "\"", n );
		if ( string::npos != np )
		{
			string url = data.substr( n, np-n );
			formatUrl( domain, url );
			addImg( url );

			m = np+1;
		}
		else
		{
			m = n;
		}

		n0 = data.find( tag, m );
	}
}

void UrlManager::formatUrl( const string& domain, string& url )
{
	size_t pos = url.find( HTTP_HEADER );
	if ( string::npos == pos )
	{
		pos = url.find( HTTPS_HEADER );
	}
	if ( string::npos == pos )
	{
		if ( '/' == url[0] )
		{
			if ( url.size() < 2 || '/' != url[1] )
			{
				url = domain + url;
			}
			else
			{
				url = "http:" + url;
			}
		}
		else
		{
			url = HTTP_HEADER + url;
		}
	}
}

bool UrlManager::popUrl( string& url )
{
	if ( _urlList.empty() )
	{
		return false;
	}

	pthread_mutex_lock( &_urlLocker );
	url = _urlList.front();
	_urlList.pop_front();
	pthread_mutex_unlock( &_urlLocker );

	return true;
}

bool UrlManager::popImg( string& url )
{
	if ( _imgList.empty() )
	{
		return false;
	}

	pthread_mutex_lock( &_imgLocker );
	url = _imgList.front();
	_imgList.pop_front();
	pthread_mutex_unlock( &_imgLocker );

	return true;
}

void UrlManager::addUrl( const string& url )
{
	pthread_mutex_lock( &_mapLocker );

	if ( _mapUsedUrl.end() != _mapUsedUrl.find( url ) )
	{
		pthread_mutex_unlock( &_mapLocker );
		return ;
	}
	_mapUsedUrl[url] = true;
	pthread_mutex_unlock( &_mapLocker );

	pthread_mutex_lock( &_urlLocker );
	_urlList.push_back(url);
	pthread_mutex_unlock( &_urlLocker );
}

void UrlManager::addImg( const string& url )
{
	pthread_mutex_lock( &_mapLocker );

	if ( _mapUsedUrl.end() != _mapUsedUrl.find( url ) )
	{
		pthread_mutex_unlock( &_mapLocker );
		return ;
	}
	_mapUsedUrl[url] = true;
	pthread_mutex_unlock( &_mapLocker );

	pthread_mutex_lock( &_imgLocker );
	_imgList.push_back(url);
	pthread_mutex_unlock( &_imgLocker );
}

string UrlManager::getDomain( const string& url )
{
	int len = HTTP_HEADER_SIZE;
	size_t pos = url.find( HTTP_HEADER );
	if ( string::npos == pos )
	{
		len = HTTPS_HEADER_SIZE;
		pos = url.find( HTTPS_HEADER );
	}
	
	string ret;
	size_t start = 0;
	if ( string::npos != pos )
	{
		start = len;
	}
	else
	{
		ret = HTTP_HEADER;
	}

	pos = url.find( "/", start );
	if ( string::npos != pos )
	{
		ret += url.substr( 0, pos );
	}
	else
	{
		ret += url;
	}

	return ret;
}
long UrlManager::getNextSn()
{
	pthread_mutex_lock( &_snLocker );
	long r = ++_sn;
	pthread_mutex_unlock( &_snLocker );

	return r;
}
