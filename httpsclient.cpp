#include "httpsclient.h"

#include <iostream>
using namespace std;

#include <openssl/err.h>

HttpsClient::HttpsClient()
{
	
}

bool HttpsClient::sendRequest(const std::string &request_str)
{
	int written_len = SSL_write(ssl, request_str.c_str(), request_str.length());
	if(written_len >= 0)
		return true;
	
	int err = SSL_get_error(ssl, written_len);
	printSendReceiveError(err);
	return false;
}

std::string HttpsClient::receivePacket()
{
	const int MAX_BYTE_READ{100};
	int read_len;
	char buffer[MAX_BYTE_READ + 1];
	string received_str;
	
	do
	{
		read_len = SSL_read(ssl, buffer, MAX_BYTE_READ);
		buffer[read_len] = 0;
		received_str += string(buffer);
	}
	while(read_len > 0);
	
	if(read_len >= 0)
		return received_str;
	
	int err = SSL_get_error(ssl, written_len);
	printSendReceiveError(err);
	return string{};
}

void HttpsClient::printConnectionError()
{
	
}

void HttpsClient::printSendReceiveError(const int err) const
{
	switch(err)
	{
		case SSL_ERROR_WANT_READ:
			cout << "Not enough data was available or still unprocessed data available at either the SSL or the BIO layer" << endl;
			break;
		case SSL_ERROR_WANT_WRITE:
			cout << "Unable to sent all data to the BIO" << endl;
			break;
		case SSL_ERROR_ZERO_RETURN:
			cout << "The TLS/SSL peer has closed the connection for writing by sending the close_notify alert. No more data can be read" << endl;
			break;
		case SSL_ERROR_SYSCALL:
			cout << "Non-recoverable, fatal I/O error occurred" << endl;
			break;
		case SSL_ERROR_SSL:
			cout << "Non-recoverable, fatal error in the SSL library occurred" << endl;
			break;
		default:
			break;
	}
}