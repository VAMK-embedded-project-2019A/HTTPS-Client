#include "httpsclient.h"

#include <iostream>
using namespace std;

#include <openssl/err.h>	// SSL_get_error()
#include <arpa/inet.h>		// htons()
#include <sys/socket.h>		// connect()

HttpsClient::HttpsClient(std::string ip, uint16_t port) : _ip{ip}, _port{port}
{
	
}

bool HttpsClient::connect()
{
	int sock = serverConnect();
	if(sock == -1)
		return false;
	
	return sslConnect(&sock);
}

bool HttpsClient::sendRequest(const std::string &request_str)
{
	int written_len = SSL_write(_ssl, request_str.c_str(), request_str.length());
	if(written_len >= 0)
		return true;
	
	int err = SSL_get_error(_ssl, written_len);
	printSendReceiveError(err);
	return false;
}

std::string HttpsClient::receiveResponse()
{
	const int MAX_BYTE_READ{100};
	int read_len;
	char buffer[MAX_BYTE_READ + 1];
	string received_str;
	
	do
	{
		read_len = SSL_read(_ssl, buffer, MAX_BYTE_READ);
		buffer[read_len] = 0;
		received_str += string(buffer);
	}
	while(read_len > 0);
	
	if(read_len >= 0)
		return received_str;
	
	int err = SSL_get_error(_ssl, read_len);
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

int HttpsClient::serverConnect() const
{
	int sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == -1)
	{
		cout << "Error creating socket" << endl;
		return -1;
	}
	
	struct sockaddr_in sock_addr;
	memset(&sock_addr, 0, sizeof(sock_addr));
	sock_addr.sin_family      = AF_INET;
	sock_addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	sock_addr.sin_port        = htons(_port);
	
	if(::connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)))
	{
		cout << "Error connecting to server" << endl;
		return -1;
	}
	
	return sock;
}

bool HttpsClient::sslConnect(int *sock)
{
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	const SSL_METHOD *method = TLSv1_2_client_method();
	
	// creates a framework to establish TLS connections
	SSL_CTX *ctx = SSL_CTX_new(method);
	if(ctx == nullptr)
	{
		cout << "Error creating CTX object" << endl;
		return false;
	}
	
	// creates a new SSL structure to hold the data for a TLS connection.
	_ssl = SSL_new(ctx);
	if(_ssl == nullptr)
	{
		cout << "Error creating SSL structure" << endl;
		printConnectionError();
		return false;
	}
	
	// if the key is self-signed, comment out this part
//	if(!SSL_CTX_check_private_key(ctx))
//	{
//		cout << "Invalid private key" << endl;
//		printConnectionError();
//		return false;
//	}
	
	// WHAT IS THIS FOR???
//	int file_descriptor = SSL_get_fd(_ssl);
//	if(file_descriptor == -1)
//	{
//		cout << "Cannot get file descriptor" << endl;
//		return false;
//	}
	
	if(!SSL_set_fd(_ssl, *sock))
	{
		cout << "Cannot set file descriptor" << endl;
		return false;
	}
	
	// initiates the TLS handshake with a server.
	int err = SSL_connect(_ssl);
	if(err <= 0)
	{
		cout << "Error creating SSL connection. Error code " << endl;
		printConnectionError();
		return false;
	}
	
	return true;
}