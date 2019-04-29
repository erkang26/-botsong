// FileName: NovBqg.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 09:24:08
#include "NovBqg.h"
#include "TsMap.h"

NovBqg::NovBqg()
: _mapUrl(NULL)
{
	_mapUrl = new TsMap();
}

NovBqg::~NovBqg()
{
	if ( NULL != _mapUrl )
	{
		delete _mapUrl;
		_mapUrl = NULL;
	}
}

string NovBqg::parseContent( const string& data )
{
	string charset = getCharset( data );

	size_t pos = data.find( "id=\"content\"" );
	if ( string::npos == pos )
	{
		return "";
	}

	pos = data.find( ">", pos );
	if ( string::npos == pos )
	{
		return "";
	}
	++pos;

	size_t pos2 = data.find( "</div>", pos );
	if ( string::npos == pos2 )
	{
		return "";
	}

	string content = data.substr( pos, pos2-pos );
	content = Utils::replace( content, "&nbsp;", " " );
	content = Utils::replace( content, "<br>", "\n" );

	content = codeTrans( charset, content );

	return content;
}

bool NovBqg::parseList( const string& data, map<string,string>& mapUrl )
{
	string charset = getCharset( data );

	size_t pos = data.find( "id=\"list\"" );
	if ( string::npos == pos )
	{
		return false; 
	}

	pos = data.find( "<dt>", pos );
	if ( string::npos == pos )
	{
		return false; 
	}

	pos = data.find( "<dt>", pos+1 );
	if ( string::npos == pos )
	{
		return false;
	}

	pos += 4;
	size_t pos2 = data.find( "</dl>", pos );
	if ( string::npos == pos )
	{
		return false;
	}

	string tmp = data.substr( pos, pos2-pos );

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
		mapUrl[url] = title;
		_mapUrl->set( url, title );
	}

	return true;
}

bool NovBqg::parseWebUrl( const string& data, map<string,string>& url )
{
	return parseList( data, url );
}
bool NovBqg::parseImgUrl( const string& data, map<string,string>& url )
{
	return false;
}
string NovBqg::getContent( const string& data )
{
	return parseContent( data );
}
void NovBqg::save( const string& url, const string& dir, const string& data )
{
	size_t pos = url.find( "//" );
	if ( string::npos != pos )
	{
		pos += 2;
	}
	else
	{
		pos = 0;
	}
	
	pos = url.find( "/", pos );
	if ( string::npos == pos )
	{
		cout<<"NovBqg save url error:"<<url<<endl;
		return ;
	}
	
	string key = url.substr( pos, url.size()-pos );

	string fileName;
	if ( !_mapUrl->get( key, fileName ) )
	{
		cout<<"can not found url key:"<<key<<endl;
		return ;
	}
	fileName = Utils::replace( fileName, " ", "_" );
	fileName += ".txt";

	ofstream fout( fileName.data() );
	if ( fout.is_open() )
	{
		string content = parseContent( data );
		fout<<content<<endl;
		fout.close();
	}
}
