#include "main.h"

#include <iostream>
using namespace std;

int main()
{
	string tag = getTagFromLocation("63.102503", "21.6182102");
	cout << "Server response: " << tag << endl;

	// we don't have any song for any weather but rain yet
	auto songs = getSongsFromTag("rain");
	cout << "Server response:" << endl;
	for(auto song : songs)
		cout << song << endl;
	cout << endl;
	
	SftpClient sftp_client{IP, "espp"};
	sftp_client.setPasswordFilePath("./test-files/password");
	sftp_client.setKnownHostsFilePath("./test-files/known-hosts");
	sftp_client.setPublicKeyFilePath("./test-files/ssh-key.pub");
	sftp_client.setPrivateKeyFilePath("./test-files/ssh-key");
	
	string file_path{"/home/espp/songs/test.txt"};
	cout << "Getting file: " << file_path << endl;
	sftp_client.getFile(file_path);

	return 0;
}

vector<SongInfo> getSongsFromTag(const string &tag)
{
	HttpsClient https_client{IP, PORT};
	if(!https_client.connect())
	{
		cout << "Cannot connect to server" << endl;
		return {};
	}

	string request{"GET /song?tag=" + tag + " HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return {};
	}
	request.erase(request.find("\r\n\r\n"));
	cout << "Request sent: " << request << endl;

	string response = https_client.receiveResponse();
	if(response.empty())
		return {};

	string data = response.substr(response.find("{"));
	SongInfoParser parser;
	parser.parseData(data);

	return parser.getSongs();
}

string getTagFromLocation(const string &longtitude, const string &latitude)
{
	HttpsClient https_client{IP, PORT};
	if(!https_client.connect())
	{
		cout << "Cannot connect to server" << endl;
		return string{};
	}

	string request{"GET /weather?longitude=" + longtitude + "&latitude=" + latitude + " HTTP/1.1\r\n\r\n"};
	if(!https_client.sendRequest(request))
	{
		cout << "Cannot send request" << endl;
		return string{};
	}
	request.erase(request.find("\r\n\r\n"));
	cout << "Request sent: " << request << endl;

	string response = https_client.receiveResponse();
	if(response.empty())
		return string{};

	string data_delim{"\r\n\r\n"};
	return response.substr(response.find(data_delim) + data_delim.length());
}
