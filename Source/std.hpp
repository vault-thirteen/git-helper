#ifndef GH_STD_H
#define GH_STD_H

#include <string>
#include <vector>

#include "libgit2/git2.h"

#define SUCCESS 0

bool is_number(const std::string& s);
std::string prettyUnixTime(git_time_t ts, int offsetMinutes);
std::vector<std::string> split_string(const std::string& s, const std::string& delimiter);
std::string toLower(std::string s);

#endif //GH_STD_H
