#include "jsonparser.h"

JsonParser::JsonParser()
{

}

void JsonParser::parse(std::string data)
{
	while(1)
	{
		SongInfo new_song;
		new_song.setName(/* whatever */);
		_song_info_vec.push_back(new_song);
	}
}