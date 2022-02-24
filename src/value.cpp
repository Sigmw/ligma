#include "value.hh"

std::string boolToString(bool v) {
    return v ? "true" : "false";
}

std::string listyToString(Value *v, char s, char e) {
    std::string res;
    res += s;
    while (v != nullptr) {
        if (!res.empty() && res[res.length() - 1] != s)
            res += " ";
        res += v->toString();
        v = v->cdr();
    }
    return res + e;
}

std::string typeStr(Type t)
{
    switch (t) {
        case Type::None: return "None";
        case Type::String: return "String";
        case Type::Int: return "Int";
        case Type::Float: return "Float";
        case Type::Symbol: return "Symbol";
        case Type::Nil: return "Nil";
        case Type::Bool: return "Bool";
        case Type::List: return "List";
        case Type::Function: return "Function";
        default: return "<INVALID>";
    };
}

Value *listValue(const Value *l)
{
    if (l == nullptr) return nullptr;
    if (!isListType(l->type())) ERROR("Not a list!");

    // FIXME Discard const
    return l->type() == Type::List
        ? toList((Value*)l)->value()
        : toVector((Value*)l)->value();
}

bool listCompartor(const Value *a, const Value *b)
{
    Value *l1 = listValue(a);
    Value *l2 = listValue(b);

    while (l1 != nullptr) {
        if (!l1->equals(l2)) return false;
        l1 = l1->cdr();
        l2 = l2->cdr();
    }
    return true;
}
