// FileName:X509Cert.h
//
// Desc:
//
// Created by token.tong at 2019-04-15 11:37:21
#ifndef _X509_CERT_H_
#define _X509_CERT_H_
#include "Utils.h"
#include "sharelib.h"
#include <openssl/ssl.h>

class X509Cert
{
public:
	X509Cert();
	~X509Cert();

	static X509Cert* parse( X509* x );

	void print();

private:
	long _ver;
	string _sn;
	string _issuerName;
	string _userName;
};

#endif //_X509_CERT_H_
