bin_PROGRAMS=botsong
botsong_SOURCES=main.cpp SocketClient.cpp X509Cert.cpp HttpRequest.cpp HttpResponse.cpp Cookie.cpp \
	    		UrlManager.cpp Scan.cpp Stat.cpp Counter.cpp cout.cpp

INCLUDES = -I./include
botsong_LDFLAGS=-L./lib -lutils -lssl -lcrypto

AUTOMAKE_OPTIONS=foreign
