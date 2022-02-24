#pragma once

#include <string>
#include <regex>

class Tokenize
{
public:
    Tokenize(std::string line);
    std::string peek() const;
    std::string next();
    bool eof() const;

private:
    void nextToken();
    void ignoreWhiteSpace();
    bool matchRegex(const std::regex &regex);

    std::string line;
    std::string::const_iterator iter;
    std::string token;
};
