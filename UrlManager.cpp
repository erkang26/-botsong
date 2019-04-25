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
	pthread_mutex_init( &_mapLocker, NULL );
	pthread_mutex_init( &_urlLocker, NULL );
	pthread_mutex_init( &_imgLocker, NULL );
	pthread_mutex_init( &_snLocker, NULL );
}

void UrlManager::parseWebUrl( const string& domainUrl, const string& data )
{
	COUT<<data<<ENDL;
	string domain = getDomain( domainUrl );
	string domainWithDir = getDomainWithDir( domainUrl );
	string tag = "href=";
	size_t n = 0;
	size_t m = 0;
	n = data.find( tag, m );
	while( string::npos != n && n <= data.size() )
	{
		n += tag.size();
		size_t np = 0;
		if ( '\"' == data[n] )
		{
			++n;
			np = data.find( "\"", n );
		}
		else
		{
			size_t np1 = data.find( ">", n );
			size_t np2 = data.find( " ", n );
			if ( string::npos == np1 )
			{
				np = np2;
			}
			else if ( string::npos == np2 )
			{
				np = np1;
			}
			else
			{
				if ( np1 <= np2 )
				{
					np = np1;
				}
				else
				{
					np = np2;
				}
			}
		}
		if ( string::npos != np )
		{
			string url = data.substr( n, np-n );
			m = np+1;
			formatUrl( domain, domainWithDir, url );
			addUrl( url );
		}
		else
		{
			m = n;
		}

		n = data.find( tag, m );
	}
}

void UrlManager::parseImgUrl( const string& domainUrl, const string& data )
{
	string domain = getDomain( domainUrl );
	string domainWithUrl = getDomainWithDir( domainUrl );

	string tag = "<img ";
	string att1 = "src=\"";
	string att2 = "lazy-src=\"";
	size_t n = 0;
	size_t m = 0;
	size_t n0 = 0;
	n0 = data.find( tag, m );
	while( string::npos != n0 && n0 <= data.size() )
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
			formatUrl( domain, domainWithUrl, url );
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

void UrlManager::formatUrl( const string& domain, const string& domainWithDir, string& url )
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
			url = domainWithDir + "/" + url;
		/*
			pos = url.find( "/" );
			string mid;
			string tmp;
			if ( string::npos != pos )
			{
				tmp = url.substr( 0, pos );
			}
			else
			{
				tmp = url;
			}
			if ( string::npos == tmp.find( "." ) )
			{
				url = domain + "/" + url;
			}
			else
			{
				url = HTTP_HEADER + url;
			}
		*/
		}
	}
}

bool UrlManager::popUrl( string& url )
{
	if ( _urlList.empty() )
	{
		//cout<<"none url"<<endl;
		return false;
	}

	pthread_mutex_lock( &_urlLocker );
	if ( _urlList.empty() )
	{
		pthread_mutex_unlock( &_urlLocker );
		//cout<<"none url"<<endl;
		return false;
	}
	url = _urlList.front();
	_urlList.pop_front();
	pthread_mutex_unlock( &_urlLocker );

	STAT_DECR( WaitingUrl );
	
	//cout<<"pop url:"<<url<<endl;

	return true;
}

bool UrlManager::popImg( string& url )
{
	if ( _imgList.empty() )
	{
		//cout<<"none image"<<endl;
		return false;
	}

	pthread_mutex_lock( &_imgLocker );
	if ( _imgList.empty() )
	{
		pthread_mutex_unlock( &_imgLocker );
		//cout<<"none image"<<endl;
		return false;
	}
	url = _imgList.front();
	_imgList.pop_front();
	pthread_mutex_unlock( &_imgLocker );

	STAT_DECR( WaitingImg );

	//cout<<"pop image:"<<url<<endl;

	return true;
}

void UrlManager::addUrl( const string& url )
{
	if ( !_sameSrc.empty() )
	{
		if ( string::npos == url.find( _sameSrc ) )
		{
			return ;
		}
	}
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

	STAT_INCR( WaitingUrl );
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

	STAT_INCR( WaitingImg );
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
string UrlManager::getDomainWithDir( const string& url )
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

	pos = url.rfind( '/' );
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

void UrlManager::print()
{
	cout<<"url:"<<endl;
	pthread_mutex_lock( &_urlLocker );
	for ( list<string>::iterator itr = _urlList.begin(); _urlList.end() != itr; ++itr )
	{
		cout<<*itr<<endl;
	}
	pthread_mutex_unlock( &_urlLocker );

	cout<<"img:"<<endl;
	pthread_mutex_lock( &_imgLocker );
	for ( list<string>::iterator itr = _imgList.begin(); _imgList.end() != itr; ++itr )
	{
		cout<<*itr<<endl;
	}
	pthread_mutex_unlock( &_imgLocker );
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
