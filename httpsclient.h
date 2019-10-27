#ifndef HTTPSCLIENT_H
#define HTTPSCLIENT_H

#include <string>

class HttpsClient
{
public:
	HttpsClient();
	
	void sendRequest();
	std::string getResponse();
	
private:

};

#endif // HTTPSCLIENT_H