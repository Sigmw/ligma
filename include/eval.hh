#pragma once

#include "value.hh"
#include "parse.hh"
#include "symbols.hh"
#include "env.hh"

class Eval
{
public:
    Eval(Symbols &s, Env &e);
    Value *eval(Parse &p);
    Value *evalValue(Value *v);

private:
    Value *evalList(ListValue *list);
    Value *evalVector(VectorValue *vector);
    Value *evalSymbol(SymbolValue *symbol);
    Value *evalFunction(FunctionValue *func, Value *n);

    Symbols &symbols;
    Env &env;
};

Value *evalLine(Symbols &s, Env &e, std::string l);
Value *evalLine(std::string l);
