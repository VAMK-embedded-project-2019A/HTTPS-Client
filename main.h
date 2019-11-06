#ifndef MAIN_H
#define MAIN_H

#include "httpsclient.h"
#include "sftpclient.h"
#include "songinfoparser.h"

#include <string>

#define IP "83.245.143.247"
#define PORT 2001

std::vector<SongInfo> getSongsFromTag(const std::string &tag);
std::string getTagFromLocation(const std::string &longtitude, const std::string &latitude);

#endif // MAIN_H