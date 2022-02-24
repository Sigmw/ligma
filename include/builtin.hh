#pragma once

#include <string>
#include "value.hh"
#include "symbols.hh"
#include "eval.hh"
#include "env.hh"

class Builtin
{
public:
    Builtin(Symbols &s);
    void init();

    void install(Env *e);
    void installFunction(Env *e, std::string f);

private:
    Symbols &symbols;
    static Value *plus(Value *a, Value *b, Eval *ev, Env *n);
    static Value *minus(Value *a, Value *b, Eval *ev, Env *n);
    static Value *mul(Value *a, Value *b, Eval *ev, Env *n);
    static Value *div(Value *a, Value *b, Eval *ev, Env *n);
    static Value *def(Value *a, Value *b, Eval *ev, Env *n);
    static Value *let_star(Value *a, Value *b, Eval *ev, Env *n);
    static Value *if_kw(Value *a, Value *b, Eval *ev, Env *n);
    static Value *do_kw(Value *a, Value *b, Eval *ev, Env *n);
    static Value *fn_star(Value *a, Value *b, Eval *ev, Env *n);
    static Value *list_is(Value *a, Value *b, Eval *ev, Env *n);
    static Value *empty_is(Value *a, Value *b, Eval *ev, Env *n);
    static Value *count(Value *a, Value *b, Eval *ev, Env *n);
    static Value *equals(Value *a, Value *b, Eval *ev, Env *n);
    static Value *less_or_eq(Value *a, Value *b, Eval *ev, Env *n);
    static Value *println(Value *a, Value *b, Eval *ev, Env *n);
};
