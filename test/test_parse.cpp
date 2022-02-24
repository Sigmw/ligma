#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"

TEST_CASE("Parse empty", "[parse]") {
    Tokenize t("");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v == nullptr);
}

TEST_CASE("Parse string", "[parse]") {
    Tokenize t("\"ab\"");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(toString(v)->value() == "ab");
}

TEST_CASE("Parse int", "[parse]") {
    Tokenize t("42");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 42);
}

TEST_CASE("Parse float", "[parse]") {
    Tokenize t("3.14159");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Float);
    REQUIRE(toFloat(v)->value() == 3.14159);
}

TEST_CASE("Parse empty list", "[parse]") {
    Tokenize t("()");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    REQUIRE(toList(v)->value() == nullptr);
    REQUIRE(toList(v)->cdr() == nullptr);
}

TEST_CASE("Parse simple list", "[parse]") {
    Tokenize t("(tst)");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    REQUIRE(toList(v)->value() != nullptr);
    REQUIRE(toList(v)->cdr() == nullptr);
}


TEST_CASE("Parse item list", "[parse]") {
    Tokenize t("(1 2)");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(v->cdr() == nullptr);

    v = toList(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(toInt(v)->value() == 1);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(toInt(v)->value() == 2);
    REQUIRE(v->cdr() == nullptr);
}

TEST_CASE("Parse list with symbols", "[parse]") {
    Tokenize t("(+ 1 2)");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "+");

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(toInt(v)->value() == 1);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(toInt(v)->value() == 2);

    v = v->cdr();
    REQUIRE(v == nullptr);
}

TEST_CASE("Parse macro", "[parse]") {
    Tokenize t("('tst)");
    Parse p(t);
    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "quote");

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "tst");

    v = v->cdr();
    REQUIRE(v == nullptr);
}

TEST_CASE("Parse constant", "[parse]") {
    Tokenize t("(false true nil)");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == false);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(toBool(v)->value() == true);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Nil);
    REQUIRE(toNil(v)->value() == nullptr);
}

TEST_CASE("Parse more complex statement", "[parse]") {
    Tokenize t("( + 2 (* 3 4))");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "+");

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 2);

    v = v->cdr();
    REQUIRE(v->type() == Type::List);

    v = toList(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "*");

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 3);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(toInt(v)->value() == 4);

    v = v->cdr();
    REQUIRE(v == nullptr);
}

TEST_CASE("Parse more complex lists", "[parse]") {
    Tokenize t("( a (b) (c) (d (e)))");

    Parse p(t);

    Value *main = p.readForm();
    REQUIRE(main != nullptr);
    REQUIRE(main->type() == Type::List);

    main = toList(main)->value();
    REQUIRE(main != nullptr);
    REQUIRE(main->type() == Type::Symbol);
    REQUIRE(toSymbol(main)->value() == "a");

    main = main->cdr();
    REQUIRE(main != nullptr);
    REQUIRE(main->type() == Type::List);

    Value *v = toList(main)->value();
    REQUIRE(v != nullptr);
    REQUIRE(toSymbol(v)->value() == "b");
    REQUIRE(v->cdr() == nullptr);

    main = main->cdr();
    REQUIRE(main != nullptr);
    REQUIRE(main->type() == Type::List);

    v = toList(main)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "c");
    REQUIRE(v->cdr() == nullptr);

    main = main->cdr();
    REQUIRE(main != nullptr);
    REQUIRE(main->type() == Type::List);
    REQUIRE(main->cdr() == nullptr);

    v = toList(main)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "d");

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(v->cdr() == nullptr);

    v = toList(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Symbol);
    REQUIRE(toSymbol(v)->value() == "e");
    REQUIRE(v->cdr() == nullptr);
}

TEST_CASE("Handle cast errors", "[parse]") {
    Tokenize t("42");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Int);

    REQUIRE_THROWS(toString(v));
    REQUIRE_NOTHROW(toInt(v));
}

TEST_CASE("Handle unbalanced list error", "[parse]") {
    Tokenize t("(");
    Parse p(t);

    REQUIRE_THROWS(p.readForm());
}

TEST_CASE("Handle unbalanced list error inside list", "[parse]") {
    Tokenize t("(()");
    Parse p(t);

    REQUIRE_THROWS(p.readForm());
}

TEST_CASE("Parse vector", "[parse]") {
    Tokenize t("[1 2]");
    Parse p(t);

    Value *v = p.readForm();
    REQUIRE(v != nullptr);
    REQUIRE(v->type() == Type::Vector);
    REQUIRE(v->cdr() == nullptr);

    v = toVector(v)->value();
    REQUIRE(v != nullptr);
    REQUIRE(toInt(v)->value() == 1);

    v = v->cdr();
    REQUIRE(v != nullptr);
    REQUIRE(toInt(v)->value() == 2);
    REQUIRE(v->cdr() == nullptr);
}
