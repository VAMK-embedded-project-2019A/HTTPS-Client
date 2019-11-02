#include "httpsclient.h"
#include <iostream>
#include <string>
using namespace std;

int main()
{
	HttpsClient https_client{"62.248.142.50", 2001};
	if(!https_client.connect())
	{
		cout << "Cannot connect to server" << endl;
		return -1;
	}
	https_client.printCerts();
	
	string request{"GET /song?tag=rain HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return -1;
	}
	cout << "Request sent: " << request;
	
	string response = https_client.receiveResponse();
	cout << "Server response" << endl << response << endl;
	
	response.find("\n\n");
	
	return 0;
}