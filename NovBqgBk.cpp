// FileName: NovBqgBk.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-29 10:43:45
#include "NovBqgBk.h"
#include "Url.h"
#include "UrlManager.h"

NovBqgBk::NovBqgBk()
{
}

NovBqgBk::~NovBqgBk()
{
}

string NovBqgBk::parseHtml( Url* parentUrl, const string& data )
{
	string charset = getCharset( data );
	do
	{
		size_t pos = data.find( "id=\"newscontent\"" );
		if ( string::npos == pos )
		{
			break;
		}

		size_t posUl = data.find( "<ul>", pos );
		if ( string::npos == posUl )
		{
			break;
		}

		size_t posUl2 = data.find( "</ul>", posUl );
		if ( string::npos == posUl2 )
		{
			break;
		}

		posUl += 4;
		string bkData = data.substr( posUl, posUl2-posUl );

		pos = posUl2;
		size_t posPg = data.find( "id=\"pagelink\"", pos );
		if ( string::npos == posPg )
		{
			break;
		}

		size_t posPg2 = data.find( "</div>", posPg );
		if ( string::npos == posPg2 )
		{
			break;
		}

		string pgData = data.substr( posPg, posPg2-posPg );

		parseBk( parentUrl, bkData, charset );
		parsePg( parentUrl, pgData, charset );
	} while(0);
	return "";
}
void NovBqgBk::save( Url* url, const string& dir, const string& data )
{
}

void NovBqgBk::parseBk( Url* parentUrl, const string& data, const string& charset )
{
	size_t pos = 0;
	size_t pos2 = 0;
	int len = strlen( "href=\"" );
	for ( ; ; )
	{
		pos2 = data.find( "class=\"s2\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}
		pos = pos2;

		pos2 = data.find( "href=\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}
		
		pos = pos2+len;
		
		pos2 = data.find( "\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		string url = data.substr( pos, pos2-pos );
		pos = pos2+1;

		pos2 = data.find( ">", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		pos = pos2+1;
		pos2 = data.find( "<", pos );
		if ( string::npos == pos2 )
		{
			break;
		}
		string name = data.substr( pos, pos2-pos );
		name = codeTrans( charset, name );
		name = Utils::replace( name, " ", "_" );
		pos = pos2;

		Url* o = new Url();
		o->setFlag( UT_NT_BQG_LIST );
		o->addArr(name);
		o->setEx(name);
		o->setUrl( parentUrl->getUrl(), url );

		UrlManager::getInstance()->addUrl(o);

	}
}
void NovBqgBk::parsePg( Url* parentUrl, const string& data, const string& charset )
{
	size_t pos = 0;
	size_t pos2 = 0;
	int len = strlen( "href=\"" );
	for ( ; ; )
	{
		pos2 = data.find( "href=\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}
		
		pos = pos2+len;
		
		pos2 = data.find( "\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		string url = data.substr( pos, pos2-pos );
		pos = pos2+1;

		Url* o = new Url();
		o->setFlag( UT_NT_BQG_BK );
		o->setUrl( parentUrl->getUrl(), url );

		UrlManager::getInstance()->addUrl(o);
	}
}
