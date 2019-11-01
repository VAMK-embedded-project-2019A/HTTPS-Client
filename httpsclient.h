#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>
#include <openssl/ssl.h>

class HttpsClient
{
public:
	HttpsClient();

	bool sendRequest(const std::string &request_str);
	std::string receivePacket(); // blocking
	void printConnectionError();
	
private:
	void printSendReceiveError(const int err) const;

	SSL *ssl;
};

#endif // HTTPSCLIENT_H