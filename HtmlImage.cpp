// FileName: HtmlImage.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 17:36:26
#include "HtmlImage.h"
#include "Url.h"
#include "UrlManager.h"

#define MIN_IMG_SIZE 10240

HtmlImage::HtmlImage()
{
}

HtmlImage::~HtmlImage()
{
}

string HtmlImage::parseHtml( Url* parentUrl, const string& data )
{
	return data;
}

void HtmlImage::save( Url* url, const string& dir, const string& data )
{
	if ( data.size() >= MIN_IMG_SIZE )
	{
		string filePath = dir + "/" + getImageName( url->getUrl() );
		ofstream fout( filePath.data(), ios::binary );
		if ( fout.is_open() )
		{
			fout.write( data.data(), data.size() );
			fout.close();
		}
	}
}

string HtmlImage::getImageName( const string& url )
{
	size_t n = url.rfind( '.' );
	string suffix;
	if ( string::npos != n )
	{
		suffix = url.substr( n, url.size()-n );
		if ( suffix.size() > 6 )
		{
			suffix.clear();
		}
	}

	char szTemp[50] = {0};
	sprintf( szTemp, "%09ld", UrlManager::getInstance()->getNextSn() );
	string file = szTemp;
	if ( suffix.empty() )
	{
		suffix = ".jpg";
	}
	file += suffix;
	return file;
}
