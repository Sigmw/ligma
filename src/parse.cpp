#include "parse.hh"

#include <regex>

static const std::regex intRegex("^[-+]?\\d+$");
static const std::regex floatRegex("^[-+]?\\d+\\.\\d*$");

Value *Parse::readForm()
{
    std::string token = tokenize.peek();
    if (token == "(") {
        tokenize.next();
        return readList(")");
    }
    if (token == "[") {
        tokenize.next();
        return readList("]");
    }
    return readAtom();
}

Value *Parse::processMacro(std::string symbol)
{
    Value *v = new SymbolValue(symbol);
    v->setNext(readForm());
    return v;
}

Value *Parse::readMacro(std::string token)
{
    if (token == "'") return processMacro("quote");
    if (token == "`") return processMacro("quasiquote");
    if (token == "@") return processMacro("deref");
    if (token == "~@") return processMacro("splice-unquote");
    if (token == "~") return processMacro("unquote");

    return nullptr;
}

Value *Parse::readConstant(std::string token)
{
    if (token == "false") return new BoolValue(false);
    if (token == "true") return new BoolValue(true);
    if (token == "nil") return new NilValue();

    return nullptr;
}

Value *Parse::readAtom()
{
    std::string token = tokenize.next();
    if (token.empty()) return nullptr;

    if (token.length() >= 2 && token[0] == '"' &&
            token[token.length() -1] == '"')
        return new StringValue(token.substr(1, token.length() - 2));

    if (std::regex_match(token, intRegex))
        return new IntValue(std::stoll(token.c_str()));

    if (std::regex_match(token, floatRegex))
        return new FloatValue(std::stod(token));

    Value *macro = readMacro(token);
    if (macro != nullptr)
        return macro;

    Value *constant = readConstant(token);
    if (constant != nullptr)
        return constant;

    return new SymbolValue(token);
}

Value *Parse::readList(std::string endMark)
{
    Value *first = nullptr;
    Value *current = nullptr;
    while (!tokenize.eof() && tokenize.peek() != endMark) {
        Value *v = readForm();
        if (v == nullptr) break;

        if (first == nullptr) first = v;
        else current->addLast(v);

        current = v;
    }
    std::string end = tokenize.next();
    if (end != endMark)
        PARSE_ERROR("Missing: " + endMark)

    if (endMark == "]") current = new VectorValue(first);
    else current = new ListValue(first);
    return current;
}
