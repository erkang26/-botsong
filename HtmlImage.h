// FileName:HtmlImage.h
//
// Desc:
//
// Created by token.tong at 2019-04-28 17:36:26
#ifndef _HTML_IMAGE_H_
#define _HTML_IMAGE_H_
#include "Utils.h"
#include "sharelib.h"
#include "UrlDelegate.h"

class HtmlImage : public UrlDelegate
{
public:
	HtmlImage();
	~HtmlImage();

	string parseHtml( Url* parentUrl, const string& data );
	void save( Url* url, const string& dir, const string& data );

private:
	string getImageName( const string& url );
};

#endif //_HTML_IMAGE_H_
