#include "UrlDelegate.h"
#include"NovBqgList.h"
#include"NovBqgContent.h"
#include "HtmlCommon.h"
#include "HtmlImage.h"
#include "NovBqgBk.h"

map<URL_TYPE, UrlDelegate*> _mapInst;

UrlDelegate* UrlDelegate::getInstance( URL_TYPE type )
{
	map<URL_TYPE, UrlDelegate*>::iterator itr = _mapInst.find( type );
	if ( _mapInst.end() != itr )
	{
		return itr->second;
	}

	UrlDelegate* p = NULL;
	switch( type )
	{
		case UT_COMMON:
			p = new HtmlCommon();
			break;

		case UT_IMAGE:
			p = new HtmlImage();
			break;

		case UT_NT_BQG_LIST:
			p = new NovBqgList();
			break;

		case UT_NT_BQG_CONTENT:
			p = new NovBqgContent();
			break;

		case UT_NT_BQG_BK:
			p = new NovBqgBk();
			break;

		default:
			break;
	}

	if ( NULL != p )
	{
		_mapInst[type] = p;
	}

	return p;
}
