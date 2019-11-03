#include "songinfoparser.h"
#include <jsoncpp/json/json.h>

std::ostream& operator<<(std::ostream& stream, const SongInfo& song_info)
{
	stream << "{" << song_info._name;
	stream << "; " << song_info._file_name;
	stream << "; " << song_info._tag << "}";
	return stream;
}

bool SongInfoParser::parseData(const std::string &data)
{
	Json::Reader reader;
	Json::Value root;
	if(!reader.parse(data, root))
	{
		std::cout << reader.getFormattedErrorMessages() << std::endl;
		return false;
	}
	
	const Json::Value items = root["songs"];
	for(unsigned int i=0; i<items.size(); i++)
	{
		SongInfo song_info;
		song_info._name 		= items[i]["name"].asString();
		song_info._file_name 	= items[i]["fileName"].asString();
		song_info._tag 			= items[i]["tag"].asString();
		_songs.push_back(song_info);
	}
	
	return true;
}

std::vector<SongInfo> SongInfoParser::getSongs() const
{
	return _songs;
}

