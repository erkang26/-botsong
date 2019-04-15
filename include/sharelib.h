/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.
*/

#ifndef _SHARE_LIB_H_
#define _SHARE_LIB_H_
#include <string>
#include "tinyxml2.h"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <assert.h>
using namespace std;

typedef tinyxml2::XMLElement XML_NODE;

class ShareLib
{
public:
	static string getString( tinyxml2::XMLElement* node, const char* name, const char* Default = "" );

	static int getInt( tinyxml2::XMLElement* node, const char* name, int Default = 0 );

	static bool getBool( tinyxml2::XMLElement* node, const char* name, bool _default = false );

	static tinyxml2::XMLDocument* loadXml( const char* fileName );
};


#endif //_SHARE_LIB_H_
