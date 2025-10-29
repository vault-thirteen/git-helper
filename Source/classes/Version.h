//
// Version is a version of a product of the git repository. It consists of
// three components: major version, minor version and patch version. The first
// symbol of the version tag is the small letter "v". Version parts are
// separated with a dot symbol `.`, all parts must be explicitly set. Example:
// "v1.23.456".
//

#ifndef GH_VERSION_H
#define GH_VERSION_H

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "../std.hpp"

class Version
{
public:
    static bool isVersion(std::string text);

    // Constructor.
    explicit Version(std::string text)
    {
        // The shortest version (e.g. "v0.0.0") has length of 6.
        if (text.size() < 6)
        {
            throw std::invalid_argument("Version raw name is too short: " + text);
        }

        // The first symbol must be a small letter "v".
        if (text[0] != 'v')
        {
            throw std::invalid_argument("Raw name is not a version: " + text);
        }

        // Version has exactly two dot symbols.
        int dotCount = static_cast<int>(std::ranges::count(text, '.'));
        if (dotCount != 2)
        {
            throw std::invalid_argument("Version parts must be separated with dots: " + text);
        }

        // Extract version data.
        std::vector<std::string> numParts = split_string(text.substr(1), ".");

        if (numParts.size() != 3)
            throw std::invalid_argument("Version parts are missing: " + text);

        for (const std::string& p : numParts)
        {
            if (!is_number(p))
                throw std::invalid_argument("Version part is not numeric: " + text);
        }

        this->major = std::stoi(numParts[0]);
        this->minor = std::stoi(numParts[1]);
        this->patch = std::stoi(numParts[2]);
    }

    bool operator<(const Version& other) const
    {
        if (this->major < other.major)
        {
            return true;
        }
        else if (this->major > other.major)
        {
            return false;
        }
        else
        {
            // Major versions are equal.
            if (this->minor < other.minor)
            {
                return true;
            }
            else if (this->minor > other.minor)
            {
                return false;
            }
            else
            {
                // Major and minor versions are equal.
                if (this->patch < other.patch)
                {
                    return true;
                }
                else if (this->patch > other.patch)
                {
                    return false;
                }
                else
                {
                    // All versions are equal.
                    return false;
                }
            }
        }
    }

    bool operator>(const Version& other) const
    {
        if (this->major > other.major)
        {
            return true;
        }
        else if (this->major < other.major)
        {
            return false;
        }
        else
        {
            // Major versions are equal.
            if (this->minor > other.minor)
            {
                return true;
            }
            else if (this->minor < other.minor)
            {
                return false;
            }
            else
            {
                // Major and minor versions are equal.
                if (this->patch > other.patch)
                {
                    return true;
                }
                else if (this->patch < other.patch)
                {
                    return false;
                }
                else
                {
                    // All versions are equal.
                    return false;
                }
            }
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Version& v)
    {
        os << v.major << "." << v.minor << "." << v.patch;
        return os;
    }

    [[nodiscard]] std::string toString() const
    {
        return std::to_string(this->major) + std::string(".") +
            std::to_string(this->minor) + std::string(".") +
            std::to_string(this->patch);
    }

    [[nodiscard]] std::string toTagName() const
    {
        return std::string("v") +
            std::to_string(this->major) + std::string(".") +
            std::to_string(this->minor) + std::string(".") +
            std::to_string(this->patch);
    }

    void incMajor()
    {
        this->major++;
    }

    void incMinor()
    {
        this->minor++;
    }

    void incPatch()
    {
        this->patch++;
    }

private:
    int major;
    int minor;
    int patch;
};

inline bool Version::isVersion(std::string text)
{
    // The shortest version (e.g. "v0.0.0") has length of 6.
    if (text.size() < 6)
    {
        return false;
    }

    // The first symbol must be a small letter "v".
    if (text[0] != 'v')
    {
        return false;
    }

    // Version has exactly two dot symbols.
    int dotCount = static_cast<int>(std::ranges::count(text, '.'));
    if (dotCount != 2)
    {
        return false;
    }

    // Extract version data.
    std::string nums = text.substr(1);
    std::vector<std::string> numParts = split_string(nums, ".");

    if (numParts.size() != 3)
        return false;

    for (const std::string& p : numParts)
    {
        if (!is_number(p))
            return false;
    }

    return true;
}

#endif //GH_VERSION_H
