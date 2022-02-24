#pragma once

#include <unordered_map>
#include "value.hh"
#include "env.hh"

class Eval;
class Symbols
{
public:
    typedef Value *(Handler)(Value *a, Value *b, Eval *ev, Env *en);
    class Symbol
    {
    public:
        Symbol(Handler *h, bool r) :
            handler(h), recurse(r) {}

        Handler *handler;
        bool recurse;
    };

    Symbols() {}
    void registerSymbol(std::string name, Handler h, bool recurse=true);
    Handler *get(std::string name);
    Symbol *getSymbol(std::string name);

private:
    std::unordered_map<std::string, Symbols::Symbol*> handlers;
};
