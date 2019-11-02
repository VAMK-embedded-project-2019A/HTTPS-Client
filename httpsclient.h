#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>
#include <openssl/ssl.h>

class HttpsClient
{
public:
	HttpsClient() = delete;
	HttpsClient(const HttpsClient &) = delete;
	HttpsClient(std::string ip, uint16_t port);

	bool connect();
	bool sendRequest(const std::string &request_str);
	std::string receiveResponse(); // blocking
	
private:
	void printConnectionError();
	void printSendReceiveError(const int err) const;
	int serverConnect() const;
	bool sslConnect(int *sock);

	SSL *_ssl{nullptr};
	std::string _ip;
	uint16_t _port;
};

#endif // HTTPSCLIENT_H