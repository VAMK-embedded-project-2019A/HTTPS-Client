#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>
#include <openssl/ssl.h>

class HttpsClient
{
public:
	HttpsClient() = delete;
	HttpsClient(const HttpsClient &) = delete;
	HttpsClient(const std::string &ip, uint16_t port);
	~HttpsClient();

	bool connect();
	
	// TODO: why the send request only works the first time
	// Temporary workaround: new object each request
	bool sendRequest(const std::string &request_str);
	std::string receiveResponse(); // blocking
	void printCerts();

private:
	void printConnectionError();
	void printSendReceiveError(const int err) const;
	bool serverConnect();
	bool sslConnect();

	SSL *_ssl{nullptr};
	SSL_CTX *_ctx{nullptr};
	int _sockfd;

	std::string _ip;
	uint16_t _port;
};

#endif // HTTPSCLIENT_H
