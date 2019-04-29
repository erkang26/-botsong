// FileName: NovBqgContent.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 16:59:09
#include "NovBqgContent.h"
#include "Url.h"
#include "cout.h"

NovBqgContent::NovBqgContent()
{
}

NovBqgContent::~NovBqgContent()
{
}

string NovBqgContent::parseHtml( Url* parentUrl, const string& data )
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
	content = codeTrans( charset, content, parentUrl->getUrl().data() );

	content = Utils::replace( content, "&nbsp;", " " );
	content = Utils::replace( content, "<br>", "\r" );
	content = Utils::replace( content, "<br />", "\r" );
	content = Utils::replace( content, "<br/>", "\r" );


	return content;
}

void NovBqgContent::save( Url* url, const string& dir, const string& data )
{
	string strDir = dir;
	const vector<string>& arr = url->getArr();
	if ( !arr.empty() )
	{
		strDir += "/";
		strDir += arr[0];
		if ( -1 == mkdir( strDir.data(), 0777 ) )
		{
			COUT<<"mkdir["<<strDir<<"] failed["<<errno<<"]"<<ENDL;
		}
	}
	string title = url->getEx();
	title = Utils::replace( title, " ", "_" );
	title += ".txt";

	string path = strDir;
	path += "/";
	path += title;

	ofstream fout( path.data() );
	if ( fout.is_open() )
	{
		fout<<data;
		fout.close();
	}

}
