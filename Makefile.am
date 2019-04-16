bin_PROGRAMS=botsong
botsong_SOURCES=main.cpp SocketClient.cpp X509Cert.cpp HttpRequest.cpp HttpResponse.cpp Cookie.cpp \
				UrlManager.cpp Scan.cpp

INCLUDES = -I./include
botsong_LDFLAGS=-L./lib -lutils -lssl -lcrypto -lhtml

AUTOMAKE_OPTIONS=foreign
