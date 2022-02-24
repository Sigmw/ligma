#include "tokenize.hh"
#include "errors.hh"

static const std::regex whitespaceRegex("[\\s,]+|;.*");
static const std::regex tokenRegexes[] = {
    std::regex("~@"),
    std::regex("[\\[\\]{}()'`~^@]"),
    std::regex("\"(?:\\\\.|[^\\\\\"])*\""),
    std::regex("[^\\s\\[\\]{}('\"`,;)]+"),
};

Tokenize::Tokenize(std::string _line)
    : line(_line), iter(line.cbegin())
{
    nextToken();
}

std::string Tokenize::peek() const
{
    return token;
}

std::string Tokenize::next()
{
    std::string ret = peek();
    nextToken();
    return ret;
}

bool Tokenize::eof() const
{
    return iter == line.cend();
}

void Tokenize::ignoreWhiteSpace()
{
    while (!eof() && matchRegex(whitespaceRegex))
        iter += token.size();
}

bool Tokenize::matchRegex(const std::regex &regex)
{
    if (eof()) return false;

    std::smatch match;
    auto flags = std::regex_constants::match_continuous;
    std::string::const_iterator end = line.cend();
    if (!std::regex_search(iter, end, match, regex, flags))
         return false;

    token = match.str(0);

    return true;
}

void Tokenize::nextToken()
{
    iter += token.size();

    ignoreWhiteSpace();
    if (eof()) {
        token = "";
        return;
    }

    for (auto &regex : tokenRegexes) {
        if (matchRegex(regex)) return;
    }

    ERROR("Mitchmatch in tokenizer\n");
}
