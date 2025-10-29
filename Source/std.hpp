#ifndef GIT1_STD_H
#define GIT1_STD_H

#include <vector>
#include "libgit2/git2.h"

#define SUCCESS 0

std::vector<std::string> splitString(const std::string& s, const std::string& delimiter);
bool is_number(const std::string& s);
std::string ToLower(std::string s);
std::string prettyTimeFromUnix(git_time_t ts, int offsetMin);

#endif //GIT1_STD_H
