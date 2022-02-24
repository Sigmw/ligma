#pragma once

#include <iostream>
#include <string>
#include "errors.hh"

enum class Type {
    None,
    String,
    Int,
    Float,
    Symbol,
    Nil,
    Bool,
    List,
    Vector,
    Function
};

inline bool isListType(const Type &t)
{
    return t == Type::List || t == Type::Vector;
}

class Value
{
public:
    Value() : valueType(Type::None), next(nullptr) {}
    virtual ~Value() {}

    Type type() const {
        return valueType;
    }
    const Value *car() const {
        return this;
    }
    Value *cdr() const {
        return next;
    }
    void addLast(Value *v) {
        Value *p = this;
        while (p->next != nullptr) p = p->next;
        p->next = v;
    }
    void setNext(Value *v) {
        next = v;
    }
    bool isList() const {
        return isListType(valueType);
    }

    virtual std::string toString() const {
        return "<INVALID>";
    }
    virtual Value* clone() const {
        return new Value(valueType);
    }
    virtual bool equals(const Value *other) const {
        if (other == nullptr) return false;
        if (!((isList() && other->isList()) ||
            valueType == other->type())) return false;

        return _equals(other);
    }

protected:
    virtual bool _equals(const Value *other) const {
        ERROR("Equals not implemented!");
    }

    Value(Type t) : valueType(t), next(nullptr) {}

private:
    Type valueType;

    Value *next;
};

typedef bool (compFunc)(const Value *a, const Value *b);
std::string typeStr(Type t);
std::string boolToString(bool v);
std::string listyToString(Value *v, char s, char e);
Value *listValue(const Value *l);
inline std::string listToString(Value *v) {
    return listyToString(v, '(', ')');
}
inline std::string vectorToString(Value *v) {
    return listyToString(v, '[', ']');
}
bool listCompartor(const Value *a, const Value *b);

#define ValueConv(x, y)\
static inline x ## Value* to##x(Value *val) {\
    if (val->type() != y)\
        ERROR("Type mismatch: got " + typeStr(val->type()) \
                + " excpected: " + typeStr(y));\
    return static_cast<x ## Value*>(val); \
}

#define ValueDef(x, y, t, s, hc, c)\
class x ## Value : public Value\
{\
public:\
    x ## Value(t v) : Value(y), _value(v) {}\
    t value() { return _value; } \
    virtual std::string toString() const {\
        return s(_value);\
    }\
    virtual Value* clone() const {\
        return new x ## Value(_value);\
    }\
protected:\
    virtual bool _equals(const Value *o) const {\
        if (hc) {\
            compFunc *ff = (compFunc*)c;\
            return (*ff)(this, o);\
        }\
        const x ## Value *other = (const x ## Value*)o;\
        return _value == other->_value;\
    }\
private:\
    t _value;\
};\
ValueConv(x, y)

ValueDef(String, Type::String, std::string, , false, nullptr)
ValueDef(Symbol, Type::Symbol, std::string, , false, nullptr)
ValueDef(Bool, Type::Bool, bool, boolToString,  false, nullptr)
ValueDef(Int, Type::Int, long long int, std::to_string, false, nullptr)
ValueDef(Float, Type::Float, double, std::to_string, false, nullptr)
ValueDef(List, Type::List, Value *, listToString, true, listCompartor)
ValueDef(Vector, Type::Vector, Value *, vectorToString, false, nullptr)

class NilValue : public Value
{
public:
    NilValue() : Value(Type::Nil) {}
    Value *value() { return nullptr; }
    std::string toString() const { return "nil"; }
    Value *clone() const { return new NilValue(); }
protected:
    virtual bool _equals(const Value *other) const {
        return false;
    }
};
ValueConv(Nil, Type::Nil)

class Env;
class FunctionValue : public Value
{
public:
    FunctionValue(Value *a, Value *b) :
        Value(Type::Function), bind(a), body(b) {}
    Value *params() { return bind; }
    Value *value() { return body; }
    std::string toString() const {
        std::string res;
        res += "#function: (fn* ";
        res += bind->toString() + " ";
        res += body->toString() + ")";
        return res;
    }
    Value *clone() const { return new FunctionValue(bind, body); }

protected:
    virtual bool _equals(const Value *other) const {
        return false;
    }
private:
    Value *bind;
    Value *body;
};
ValueConv(Function, Type::Function)

inline Value *iterInit(Value *o) {
    if (o->type() != Type::List && o->type() != Type::Vector)
        FATAL("Invalid iterator");
    return o->type() == Type::List
        ? toList(o)->value()
        : toVector(o)->value();
}

inline Value *iterNext(Value *v) {
    if (v == nullptr) return nullptr;
    return v->cdr();
}
