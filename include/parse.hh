#pragma once

#include "tokenize.hh"
#include "value.hh"

class Parse
{
public:
    Parse(Tokenize &t) : tokenize(t) {}
    Value *readForm();
    Value *readAtom();
    Value *readList(std::string endMark);
    Value *readMacro(std::string token);
    Value *processMacro(std::string symbol);
    Value *readConstant(std::string token);

private:
    Tokenize &tokenize;
};

