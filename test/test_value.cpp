#include "catch.hpp"
#include "tokenize.hh"
#include "parse.hh"
#include "value.hh"

TEST_CASE("Value", "[value]") {
    Value *v = new Value();
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::None);
    REQUIRE(v->toString() == "<INVALID>");
}

TEST_CASE("String value", "[value]") {
    StringValue *v = new StringValue("tst");
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::String);
    REQUIRE(v->value() == "tst");
    REQUIRE(v->toString() == "tst");
}

TEST_CASE("Int value", "[value]") {
    IntValue *v = new IntValue(42);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::Int);
    REQUIRE(v->value() == 42);
    REQUIRE(v->toString() == "42");
}

TEST_CASE("Bool value false", "[value]") {
    BoolValue *v = new BoolValue(false);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(v->value() == false);
    REQUIRE(v->toString() == "false");
}

TEST_CASE("Bool value true", "[value]") {
    BoolValue *v = new BoolValue(true);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::Bool);
    REQUIRE(v->value() == true);
    REQUIRE(v->toString() == "true");
}

TEST_CASE("Nil value", "[value]") {
    NilValue *v = new NilValue();
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::Nil);
    REQUIRE(v->value() == nullptr);
    REQUIRE(v->toString() == "nil");
}

TEST_CASE("Empty list value", "[value]") {
    ListValue *v = new ListValue(nullptr);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(v->value() == nullptr);
    REQUIRE(v->toString() == "()");
}

TEST_CASE("List value with one value", "[value]") {
    StringValue *s = new StringValue("tst");

    ListValue *v = new ListValue(s);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(v->value() == s);
    REQUIRE(v->toString() == "(tst)");
}

TEST_CASE("List value with two valuee", "[value]") {
    StringValue *s = new StringValue("tst");
    IntValue *i = new IntValue(42);
    s->addLast(i);

    ListValue *v = new ListValue(s);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(v->value() == s);
    REQUIRE(v->value()->cdr() == i);
    REQUIRE(v->toString() == "(tst 42)");
}

TEST_CASE("List within list", "[value]") {
    StringValue *s = new StringValue("tst");
    IntValue *i = new IntValue(42);
    ListValue *l = new ListValue(i);
    s->addLast(l);

    ListValue *v = new ListValue(s);
    REQUIRE(v != nullptr);
    REQUIRE(v->cdr() == nullptr);
    REQUIRE(v->type() == Type::List);
    REQUIRE(v->value() == s);
    REQUIRE(v->value()->cdr() == l);
    REQUIRE(toList(v->value()->cdr())->value() == i);
    REQUIRE(v->toString() == "(tst (42))");
}
