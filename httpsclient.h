#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <cstdlib>
#include <jsoncpp/json/json.h>

using namespace std;

SSL *ssl;
int sock;

class HttpsClient
{
	public:
		HttpsClient();
	
		void SendPacket();
		std::string RecvPacket();
		void log_ssl();
		void ShowCerts(SSL* ssl);
		void connect_https();
	private:
};

#endif // HTTPSCLIENT_H