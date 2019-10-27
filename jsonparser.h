#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <string>
#include <vector>

class JsonParser
{
public:
	JsonParser();
	
	void parse(std::string data);
	std::vector getSongs();
	
private:
	std::vector<SongInfo> _song_info_vec;
};

#endif // JSONPARSER_H