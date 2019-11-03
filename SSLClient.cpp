#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <curl/curl.h>
#include <cstdlib>
#include <jsoncpp/json/json.h>

using namespace std;

SSL *ssl;
int sock;

std::string RecvPacket()
{
	int len=100;
	char buf[10000];
	std::string Str;
	do
	{
		//int SSL_read(SSL *ssl, void *buf, int num): read num bytes from the specified ssl into the buffer buf. 
		//Return number of bytes actually read from the TLS/SSL connection else return number between 0 and -1 indicate error
		len=SSL_read(ssl, buf, 100);
		buf[len]=0;
		//printf(buf);
		Str += std::string(buf);
	}
	while (len > 0);
	if (len < 0)
	{
		//int SSL_get_error(const SSL *ssl, int ret): returns a result code for a preceding call to SSL_read() on ssl
		int err = SSL_get_error(ssl, len);
		if (err == SSL_ERROR_WANT_READ)
			printf("not enough data was available or still unprocessed data available at either the SSL or the BIO layer\n");
		if (err == SSL_ERROR_WANT_WRITE)
			printf("unable to sent all data to the BIO\n");
		if (err == SSL_ERROR_ZERO_RETURN)
			printf("The TLS/SSL peer has closed the connection for writing by sending the close_notify alert. No more data can be read\n");
		if (err == SSL_ERROR_SYSCALL)
			printf("non-recoverable, fatal I/O error occurred\n");
		if (err == SSL_ERROR_SSL)
			printf("non-recoverable, fatal error in the SSL library occurred\n");
	}
	return Str;
}

int SendPacket(const char *buf)
{
	//int SSL_write(SSL *ssl, const void *buf, int num): write num bytes from the buffer buf into the specified ssl connection.
	//will return 1 for success or number between 0 and -1 indicate error.
	int len = SSL_write(ssl, buf, strlen(buf));
	if (len < 0)
	{
		int err = SSL_get_error(ssl, len);
		switch (err)
		{
			case SSL_ERROR_WANT_WRITE:
				return 0;
			case SSL_ERROR_WANT_READ:
				return 0;
			case SSL_ERROR_ZERO_RETURN:
			case SSL_ERROR_SYSCALL:
			case SSL_ERROR_SSL:
            default:
				return -1;
		}
	}
}


void log_ssl()
{
	int err;
	while (err = ERR_get_error())
	{
		//char *ERR_error_string(unsigned long e, char *buf): generates a human-readable string representing the error code e, and places it at buf.
		//returns a pointer to a static buffer containing the string.
		char *str = ERR_error_string(err, 0);
		if (!str)
			return;
		printf(str);
		printf("\n");
		fflush(stdout);
	}
}

void ShowCerts(SSL* ssl)
{
    X509 *cert;
    char *line;
    cert = SSL_get_peer_certificate(ssl);
    if ( cert != NULL )
    {
        printf("Server certificates:\n");
        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);
        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);
        X509_free(cert);
    }
    else
        printf("Info: No client certificates configured.\n");
}

void connect_https()
{
	int s;
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (!s)
	{
		printf("Error creating socket.\n");
		//return -1;
	}
	struct sockaddr_in sa;
	memset (&sa, 0, sizeof(sa));
	sa.sin_family      = AF_INET;
	sa.sin_addr.s_addr = inet_addr("62.248.142.50");
	sa.sin_port        = htons (2001);
	socklen_t socklen = sizeof(sa);
	if (connect(s, (struct sockaddr *)&sa, socklen))
	{
		printf("Error connecting to server.\n");
		//return -1;
	}
	SSL_library_init();
	SSLeay_add_ssl_algorithms();
	SSL_load_error_strings();
	const SSL_METHOD *meth = TLSv1_2_client_method();
	//creates a new SSL_CTX object as framework to establish TLS/SSL or DTLS enabled connections.
	//return NULL when the creation of a new SSL_CTX object failed. return value points to an allocated SSL_CTX object.
	SSL_CTX *ctx = SSL_CTX_new (meth);
	//creates a new SSL structure which is needed to hold the data for a TLS/SSL connection.
	//return NULL if creation of a new SSL structure failed. Else return value points to an allocated SSL structure.
	ssl = SSL_new (ctx);
	/*if(!SSL_CTX_check_private_key(ctx))
	{
		printf("Invalid private key.\n");
		log_ssl();
		abort();
	}
	if (!ssl)
 	{
		printf("Error creating SSL.\n");
		log_ssl();
		//return -1;
	}*/
	//returns the file descriptor which is linked to ssl.
	//return -1 if operation failed. Else return file descriptor linked to ssl.
	sock = SSL_get_fd(ssl);
	//int SSL_set_fd(SSL *ssl, int fd): sets the file descriptor fd as the input/output facility for the TLS/SSL (encrypted) side of ssl. fd will typically be the socket file descriptor of a network connection.
	//return 0 if operation failed or 1 if operation succeeded.
	SSL_set_fd(ssl, s);
	//initiates the TLS/SSL handshake with a server.
	//return 0 if TLS/SSL handshake was not successful but was shut down controlled and by the specifications of the TLS/SSL protocol.
	//return 1 if TLS/SSL handshake was successfully completed, a TLS/SSL connection has been established.
	//return <0 if TLS/SSL handshake was not successful, because a fatal error occurred either at the protocol level or a connection failure occurred. The shutdown was not clean. 
	int err = SSL_connect(ssl);
	if (err <= 0)
	{
		printf("Error creating SSL connection.  err=%x\n", err);
		log_ssl();
		fflush(stdout);
		//return -1;
	}
}

void parse_data(std::string data)
{
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(data, root))
	{
		std::cout << reader.getFormattedErrorMessages();
		exit(1);
	}
	else
	{
		const Json::Value mynames = root["songs"];
		for ( int index = 0; index < mynames.size(); ++index )
		{
			std::cout << std::endl << "Name: " << mynames[index]["name"] << std::endl;
			std::cout << "File Name: " << mynames[index]["fileName"] << std::endl;
			std::cout << "Tag: " << mynames[index]["tag"] << std::endl;
		}
	}
}

//SFTP get file from server
/*
	Follow this link to generate key pair and upload public key to the server (follow all the steps except for step 5)
		https://community.atlassian.com/t5/Bitbucket-questions/How-do-I-set-up-ssh-public-key-authentication-so-that-I-can-use/qaq-p/171671
	Follow this link if you have problem with SHH host key:
		https://serverfault.com/questions/783648/why-cant-curl-retrieve-the-ssh-host-key-key-none
*/
#undef DISABLE_SSH_AGENT
struct FtpFile
{
	const char *filename;
	FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FtpFile *out = (struct FtpFile *)stream;
	if(!out->stream) {
    /* open file for writing */
	out->stream = fopen(out->filename, "wb");
	if(!out->stream)
		return -1; /* failure, can't open file to write */
	}
	return fwrite(buffer, size, nmemb, out->stream);
}

void sftp_get()
{
	CURL *curl;
	CURLcode res;
	struct FtpFile ftpfile = 
	{
		"test_sftp.txt", /* name to store the file as if successful */
		NULL
	};
	curl_global_init(CURL_GLOBAL_DEFAULT);
	curl = curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL,"sftp://espp@62.248.142.50/home/espp/songs/test.txt");
		curl_easy_setopt(curl, CURLOPT_USERNAME, "espp"); 
		/* Define our callback to get called when there's data to be written */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
		/* Set a pointer to our struct to pass to the callback */
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

	#ifndef DISABLE_SSH_AGENT
    	/* We activate ssh agent. For this to work you need
       	to have ssh-agent running (type set | grep SSH_AGENT to check) or
       	pageant on Windows (there is an icon in systray if so) */
    	curl_easy_setopt(curl, CURLOPT_SSH_AUTH_TYPES, CURLSSH_AUTH_PUBLICKEY | CURLSSH_AUTH_PASSWORD); // CURLSSH_AUTH_PUBLICKEY |  
    	curl_easy_setopt(curl, CURLOPT_SSH_KNOWNHOSTS, "/home/pi/.ssh/my_known_hosts"); 
		curl_easy_setopt(curl, CURLOPT_SSH_PUBLIC_KEYFILE, "/home/pi/.ssh/my_ssh_key.pub");
		curl_easy_setopt(curl, CURLOPT_SSH_PRIVATE_KEYFILE, "/home/pi/.ssh/my_ssh_key"); 
    	curl_easy_setopt(curl, CURLOPT_KEYPASSWD,"pass123");
	#endif
		/* Switch on full protocol/debug output */
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		res = curl_easy_perform(curl);
		/* always cleanup */
		curl_easy_cleanup(curl);
		if(CURLE_OK != res)
		{
			/* we failed */
			fprintf(stderr, "curl told us %d\n", res);
		}
	}

	if(ftpfile.stream)
		fclose(ftpfile.stream); /* close the local file */
	curl_global_cleanup();
}

int main(int argc, char *argv[])
{
	connect_https();
	ShowCerts(ssl);
	char *request = "GET /song?tag=rain HTTP/1.1\r\n\r\n";
	SendPacket(request);
	std::string test = RecvPacket();
	std::string data = test.substr(213);
	parse_data(data);
	sftp_get();
	return 0;
}