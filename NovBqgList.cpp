// FileName: NovBqgList.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 15:10:27
#include "NovBqgList.h"
#include "Url.h"
#include "UrlManager.h"

NovBqgList::NovBqgList()
{
}

NovBqgList::~NovBqgList()
{
}

string NovBqgList::parseHtml( Url* parentUrl, const string& data )
{
	string charset = getCharset( data );

	size_t pos = data.find( "id=\"list\"" );
	if ( string::npos == pos )
	{
		return ""; 
	}

	pos = data.find( "<dt>", pos );
	if ( string::npos == pos )
	{
		return ""; 
	}

	pos = data.find( "<dt>", pos+1 );
	if ( string::npos == pos )
	{
		return "";
	}

	pos += 4;
	size_t pos2 = data.find( "</dl>", pos );
	if ( string::npos == pos )
	{
		return "";
	}

	string tmp = data.substr( pos, pos2-pos );

	UrlManager* mg = UrlManager::getInstance();
	pos = 0;
	for ( ; ; )
	{
		pos2 = tmp.find( "<a", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		pos = pos2;
		pos2 = tmp.find( "href=\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}
	
		pos = pos2 + strlen( "href=\"" );
		pos2 = tmp.find( "\"", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		string url = tmp.substr( pos, pos2-pos );
		pos = pos2;
		pos2 = tmp.find( ">", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		pos = pos2+1;
		pos2 = tmp.find( "</a>", pos );
		if ( string::npos == pos2 )
		{
			break;
		}

		string title = tmp.substr( pos, pos2-pos );
		pos = pos2 + strlen( "<a>" );

		title = codeTrans( charset, title );

		Url* u = new Url( *parentUrl );
		u->setUrl( parentUrl->getUrl(), url );
		u->setFlag( UT_NT_BQG_CONTENT );
		u->setEx( title );
		u->addArr( title );
		mg->addUrl(u);
	}

	return "";
}
void NovBqgList::save( Url* url, const string& dir, const string& data )
{
}
