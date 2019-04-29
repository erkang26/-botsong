// FileName: HtmlCommon.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 17:19:03
#include "HtmlCommon.h"
#include "Url.h"
#include "UrlManager.h"

HtmlCommon::HtmlCommon()
{
}

HtmlCommon::~HtmlCommon()
{
}

string HtmlCommon::parseHtml( Url* parentUrl, const string& data )
{
	parseWebUrl( parentUrl->getUrl(), data );
	parseImgUrl( parentUrl->getUrl(), data );
	return data;
}
void HtmlCommon::save( Url* url, const string& dir, const string& data )
{
	string htmlFile = dir + "/";
	htmlFile += getFileByUrl( url->getUrl() );
	htmlFile += ".html";
	ofstream fout( htmlFile.data() );
	if ( fout.is_open() )
	{
		fout<<data;
		fout.close();
	}
}
string HtmlCommon::getFileByUrl( const string& url )
{
	string tmp = url;
	if ( 0 == tmp.find( HTTP_HEADER ) )
	{
		tmp.erase( 0, HTTP_HEADER_SIZE );
	}

	Utils::trimAll( tmp );

	return tmp;
}
void HtmlCommon::parseWebUrl( const string& domainUrl, const string& data )
{
	COUT<<data<<ENDL;
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
			Url* u = new Url();
			u->setUrl( domainUrl, url );
			u->setFlag(UT_COMMON);
			UrlManager::getInstance()->addUrl(u);
		}
		else
		{
			m = n;
		}

		n = data.find( tag, m );
	}
}
void HtmlCommon::parseImgUrl( const string& domainUrl, const string& data )
{
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
			Url* u = new Url;
			u->setUrl( domainUrl, url );
			u->setFlag( UT_IMAGE );
			UrlManager::getInstance()->addImg( u );

			m = np+1;
		}
		else
		{
			m = n;
		}

		n0 = data.find( tag, m );
	}
}
