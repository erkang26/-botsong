bin_PROGRAMS=botsong
botsong_SOURCES=main.cpp SocketClient.cpp X509Cert.cpp HttpRequest.cpp HttpResponse.cpp Cookie.cpp \
	    		UrlManager.cpp Scan.cpp Stat.cpp Counter.cpp cout.cpp NovBase.cpp Locker.cpp \
				TsMap.cpp TsList.cpp UrlDelegate.cpp NovBqgList.cpp NovBqgContent.cpp HtmlCommon.cpp \
				HtmlImage.cpp Url.cpp NovBqgBk.cpp

INCLUDES = -I./include
botsong_LDADD=-L./lib -lssl -lcrypto -liconv -lpthread -lutils

AUTOMAKE_OPTIONS=foreign
