// FileName: X509Cert.cpp
//
// Desc:
//
// Created by token.tong at 2019-04-15 11:37:21
#include "X509Cert.h"
#include "cout.h"

X509Cert::X509Cert()
: _ver(0)
{
}

X509Cert::~X509Cert()
{
}

X509Cert* X509Cert::parse( X509* x )
{
	X509Cert* ct = NULL;
	BIGNUM* bignum = NULL;
	char* sn = NULL;
	bool ret = false;

	do
	{
		ct = new X509Cert;
		ct->_ver = X509_get_version( x );

		{
			ASN1_INTEGER* asnInt = X509_get_serialNumber( x );
			bignum = ASN1_INTEGER_to_BN( asnInt, NULL );
			if ( NULL == bignum )
			{
				CERR<<"bignum error"<<ENDL;
				break;
			}

			sn = BN_bn2hex( bignum );
			if ( NULL == sn )
			{
				CERR<<"sn error"<<ENDL;
				break;
			}
			
			ct->_sn = sn;
		}

		{
			X509_NAME* pName = X509_get_issuer_name( x );
			if ( NULL == pName )
			{
				CERR<<"issuer error"<<ENDL;
				break;
			}
			ct->_issuerName = X509_NAME_oneline( pName, NULL, 0 );
		}

		{
			X509_NAME* pName = X509_get_subject_name( x );
			if ( NULL == pName )
			{
				CERR<<"subject error"<<ENDL;
				break;
			}

			ct->_userName = X509_NAME_oneline( pName, NULL, 0 );
		}

		ret = true;
		goto FREE;
	} while(0);

FREE:
	if ( NULL != sn )
	{
		OPENSSL_free( sn );
		sn = NULL;
	}

	if ( NULL != bignum )
	{
		BN_free( bignum );
		bignum = NULL;
	}
	

	if ( !ret && NULL != ct )
	{
		delete ct;
		ct = NULL;
	}	

	return ct;
}

void X509Cert::print()
{
	CRUN<<"ver:"<<_ver<<ENDL;
	CRUN<<"sn:"<<_sn<<ENDL;
	CRUN<<"issuer:"<<_issuerName<<ENDL;
	CRUN<<"subject:"<<_userName<<ENDL;
}
