#include "httpsclient.h"
#include "songinfoparser.h"

#include <iostream>
#include <string>
using namespace std;

vector<SongInfo> getSongsFromTag(HttpsClient *https_client, string tag)
{
	string request{"GET /song?tag=" + tag + " HTTP/1.1\r\n\r\n"};
	if(!https_client->sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return {};
	}
	cout << "Request sent: " << request;

	string response = https_client->receiveResponse();
	cout << "Server response" << endl << response << endl;

	string data = response.substr(response.find("{"));
	SongInfoParser parser;
	parser.parseData(data);

	return parser.getSongs();
}

string getTagFromLocation(HttpsClient *https_client, string location)
{
	string request{"GET /" + location + " HTTP/1.1\r\n\r\n"};
	if(!https_client->sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return {};
	}
	cout << "Request sent: " << request;

	string response = https_client->receiveResponse();
	cout << "Server response" << endl << response << endl;
	return response;
}

int main()
{
	HttpsClient https_client{"62.248.142.50", 2001};
	if(!https_client.connect())
	{
		cout << "Cannot connect to server" << endl;
		return -1;
	}
	https_client.printCerts();

//	auto songs = getSongsFromTag(&https_client, "rain");
//	for(auto song : songs)
//		cout << song << endl;

	getTagFromLocation(&https_client, "63.102503,21.6182102");

	return 0;
}
