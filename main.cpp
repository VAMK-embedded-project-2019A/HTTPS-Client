#include "httpsclient.h"
#include "songinfoparser.h"

#include <iostream>
#include <string>
using namespace std;

vector<SongInfo> getSongsFromTag(const string &tag)
{
	HttpsClient https_client{"62.248.142.50", 2001};
	if(!https_client.connect())
	{
		cout << "Cannot connect to server" << endl;
		return {};
	}
	https_client.printCerts();

	string request{"GET /song?tag=" + tag + " HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return {};
	}
	cout << "Request sent: " << request;

	string response = https_client.receiveResponse();
	cout << "Server response" << endl << response << endl;
	if(response.empty())
		return {};

	string data = response.substr(response.find("{"));
	SongInfoParser parser;
	parser.parseData(data);

	return parser.getSongs();
}

string getTagFromLocation(const string &longtitude, const string &latitude)
{
	HttpsClient https_client{"62.248.142.50", 2001};
	if(!https_client.connect())
	{
		cout << "Cannot connect to server" << endl;
		return string{};
	}
	https_client.printCerts();

	string request{"GET /weather?longitude=" + longtitude + "&latitude=" + latitude + " HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return string{};
	}
	cout << "Request sent: " << request;

	string response = https_client.receiveResponse();
	cout << "Server response" << endl << response << endl;
	if(response.empty())
		return string{};

	string data_delim{"\r\n\r\n"};
	return response.substr(response.find(data_delim) + data_delim.length());
}

int main()
{
	string tag = getTagFromLocation("63.102503", "21.6182102");
	if(tag.empty())
		return -1;

	// we don't have any song for any weather but rain yet
	auto songs = getSongsFromTag("rain");
	for(auto song : songs)
		cout << song << endl;

	return 0;
}
