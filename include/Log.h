/*
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.
*/

#ifndef _LOG_H_
#define _LOG_H_
#include <string>
#include <vector>
using namespace std;

class CLogPrinter
{
public:
	virtual void logPrint() = 0;
};

class CLog
{
public:
	CLog();
	~CLog();

	void add( const string& s ) { _logs.push_back(s); }
	void print();
	void reset() { _logs.clear(); }
	void setPrinter( CLogPrinter* printer ) { _printer = printer; }
private:
	vector<string> _logs;
	CLogPrinter* _printer;
};

extern CLog __log__;

#define LOG_ADD( __log__ ) _log.add( __log__ )
#define LOG_RESET() _log.reset()
#define LOG_SET_PRINTER( __printer__ ) _log.setPrinter( __printer__ )

#endif //_LOG_H_
