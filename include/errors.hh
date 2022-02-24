#pragma once

#include <string>

#define THROW(p, x) throw std::string(p) + \
    " " + std::string(x);

#define DEBUG_THROW(p, x) throw std::string(p) + \
    " " + std::string(__FILE__) +\
    "@" +std::to_string(__LINE__) + \
    " " + std::string(x);


#define FATAL(x) DEBUG_THROW("FATAL:", x)
#define ERROR(x) THROW("ERROR:", x)
#define PARSE_ERROR(x) THROW("PARSE ERROR:", x)
