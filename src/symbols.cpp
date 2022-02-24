#include "symbols.hh"

void Symbols::registerSymbol(std::string name, Handler h, bool recurse)
{
    handlers[name] = new Symbol(h, recurse);
}

Symbols::Handler *Symbols::get(std::string name)
{
    auto res = handlers.find(name);
    if (res == handlers.end()) return nullptr;

    return res->second->handler;
}

Symbols::Symbol *Symbols::getSymbol(std::string name)
{
    auto res = handlers.find(name);
    if (res == handlers.end()) return nullptr;

    return res->second;
}
