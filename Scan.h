// FileName:Scan.h
//
// Desc:
//
// Created by token.tong at 2019-04-16 17:19:51
#ifndef _SCAN_H_
#define _SCAN_H_
#include "Utils.h"
#include "sharelib.h"

class UrlManager;

class Scan
{
public:
	Scan();
	~Scan();

	void start();

private:
	static void* threadEntry( void* p );
	void thread();

	void downloadImg( const string& url );
	void downloadWeb( UrlManager* mg, const string& url );

	string getFileByUrl( const string& rootDir, const string& url );
	string getImageName( string url );
};

#endif //_SCAN_H_
