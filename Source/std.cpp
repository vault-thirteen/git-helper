#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <format>
#include <algorithm>
#include "std.hpp"
#include "libgit2/git2.h"

std::vector<std::string> splitString(const std::string& s, const std::string& delimiter)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = s.find(delimiter);

    while (end != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
        end = s.find(delimiter, start);
    }

    tokens.push_back(s.substr(start)); // Add the last token.

    return tokens;
}

bool is_number(const std::string& s)
{
    return !s.empty() && std::ranges::find_if(s, [](unsigned char c) { return !std::isdigit(c); }) == s.end();
}

std::string ToLower(std::string s)
{
    std::ranges::transform(s, s.begin(),
                           [](unsigned char c) { return std::tolower(c); });
    return s;
}

std::string prettyTimeFromUnix(git_time_t ts, int offsetMin)
{
    auto tp = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::from_time_t(ts));
    std::stringstream ss;
    ss << std::format("{:%Y-%m-%d %H:%M:%S}", tp);

    // Time zone.
    int offsetHrs = offsetMin / 60;
    if (offsetHrs > 0)
    {
        ss << " (UTC+" << offsetHrs << ")";
    }
    else
    {
        ss << " (UTC-" << offsetHrs << ")";
    }

    return ss.str();
}
