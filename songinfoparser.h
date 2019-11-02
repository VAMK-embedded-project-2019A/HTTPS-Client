#ifndef SONGINFOPARSER_H
#define SONGINFOPARSER_H

#include <string>
#include <vector>
#include <iostream>

struct SongInfo
{
	std::string _name;
	std::string	_file_name;
	std::string _tag;
};

std::ostream& operator<<(std::ostream& stream, const SongInfo& song_info);

class SongInfoParser
{
public:
	SongInfoParser() = default;

	bool parseData(const std::string &data);
	std::vector<SongInfo> getSongs() const;
	
private:
	std::vector<SongInfo> _songs;
};

#endif // SONGINFOPARSER_H