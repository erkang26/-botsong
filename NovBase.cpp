// FileName: NovBase.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-28 09:39:43
#include "NovBase.h"
#include "iconv.h"
#include "cout.h"

NovBase::NovBase()
{
}

NovBase::~NovBase()
{
}

string NovBase::getCharset( const string& data )
{
	size_t pos = data.find( "http-equiv=\"Content-Type\"" );
	if ( string::npos == pos )
	{
		return "";
	}

	pos = data.find( "content=\"", pos );
	if ( string::npos == pos )
	{
		return "";
	}

	pos += strlen( "content=\"" );
	size_t pos2 = data.find( "\"", pos );
	if ( string::npos == pos2 )
	{
		return "";
	}

	string tmp = data.substr( pos, pos2-pos );
	pos = tmp.find( "charset=" );
	if ( string::npos == pos )
	{
		return "";
	}

	pos += strlen( "charset=" );
	string ret = tmp.substr( pos, tmp.size()-pos );

	ret = Utils::lowerCase(ret);

	return ret;
}

string NovBase::codeTrans( const string& charset, const string& data, const char* ex )
{
	if ( "utf-8" == charset || "utf8" == charset )
	{
		return data;
	}
	COUT<<data<<ENDL;
	COUT<<data.size()<<ENDL;
	size_t inLen = data.size();
	char* inBuf = (char*)data.data();
	size_t outLen = inLen*10;
	char* outBuf = new char[outLen];
	memset( outBuf, 0, outLen );
	char* out = outBuf;
	iconv_t cd = 0;
	string ret = data;

	COUT<<inLen<<ENDL;
	COUT<<outLen<<ENDL;
	do
	{
		if ( "gbk" == charset )
		{
			cd = iconv_open( "utf-8", "cp936");
			if ( 0 == cd )
			{
				CERR<<"open gbk to utf-8 failed"<<ENDL;
				break;
			}
		}

		int n = 0;
		if ( -1  == (n=iconv( cd, &inBuf, &inLen, &outBuf, &outLen )) )
		{
			COUT<<outLen<<ENDL;
			COUT<<out<<ENDL;
			CERR<<"["<<charset<<"]iconv from gbk to utf-8 failed: "<<errno<<"("<<ex<<")"<<ENDL;
			break;
		}
		outLen = strlen(out);
		COUT<<outLen<<" - "<<n<<ENDL;

		ret.clear();
		ret.append( out, outLen );
	} while(0);

	COUT<<outLen<<ENDL;

	if ( NULL != out )
	{
		delete [] out;
		outBuf = NULL;
		out = NULL;
	}

	if ( 0 != cd )
	{
		iconv_close(cd);
		cd = 0;
	}

	return ret;
}
