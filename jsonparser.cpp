#include "jsonparser.h"

JsonParser::JsonParser()
{

}

void JsonParser::parse(std::string data)
{
	Json::Reader reader;
    Json::Value root;
    if(!reader.parse(data, root))
	{
        std::cout << reader.getFormattedErrorMessages();
        exit(1);
    }
    else
	{
		const Json::Value mynames = root["songs"];
		for ( int index = 0; index < mynames.size(); ++index )
		{
			std::cout << std::endl << "Name: " << mynames[index]["name"] << std::endl;
			std::cout << "File Name: " << mynames[index]["fileName"] << std::endl;
			std::cout << "Tag: " << mynames[index]["tag"] << std::endl;
		}
	}
}