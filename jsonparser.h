#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <cstdlib>
#include <jsoncpp/json/json.h>

using namespace std;

class JsonParser
{
	public:
		JsonParser();
	
		void parse_data(std::string data);
		//std::vector getSongs();	
	private:
		//std::vector<SongInfo> _song_info_vec;
};

#endif // JSONPARSER_H