#include "catch.hpp"
#include "tokenize.hh"

TEST_CASE("Tokenize empty", "[token]") {
    Tokenize t("");
    REQUIRE(t.eof());
}

TEST_CASE("Tokenize empty list", "[token]") {
    Tokenize t("()");
    REQUIRE(!t.eof());
    REQUIRE(t.next() == "(");
    REQUIRE(t.next() == ")");
    REQUIRE(t.eof());
}

TEST_CASE("Tokenize plus", "[token]") {
    Tokenize t("(+ 1 2 3)");
    REQUIRE(!t.eof());
    REQUIRE(t.next() == "(");
    REQUIRE(t.next() == "+");
    REQUIRE(t.next() == "1");
    REQUIRE(t.next() == "2");
    REQUIRE(t.next() == "3");
    REQUIRE(t.next() == ")");
    REQUIRE(t.eof());
}

TEST_CASE("Tokenize list inside list", "[token]") {
    Tokenize t("(+ 1 (+ 2 3))");
    REQUIRE(!t.eof());
    REQUIRE(t.next() == "(");
    REQUIRE(t.next() == "+");
    REQUIRE(t.next() == "1");
    REQUIRE(t.next() == "(");
    REQUIRE(t.next() == "+");
    REQUIRE(t.next() == "2");
    REQUIRE(t.next() == "3");
    REQUIRE(t.next() == ")");
    REQUIRE(t.next() == ")");
    REQUIRE(t.eof());
}

TEST_CASE("Tokenize peek", "[token]") {
    Tokenize t("(+ 1 2)");
    REQUIRE(!t.eof());
    REQUIRE(t.next() == "(");
    REQUIRE(t.peek() == "+");
    REQUIRE(t.peek() == "+");
    REQUIRE(t.next() == "+");
    REQUIRE(t.next() == "1");
    REQUIRE(!t.eof());
}

TEST_CASE("Don't tokenize comment", "[token]") {
    Tokenize t("1 2 ; comment");
    REQUIRE(!t.eof());
    REQUIRE(t.next() == "1");
    REQUIRE(t.next() == "2");
    REQUIRE(t.eof());
}

TEST_CASE("Tokenize white space", "[token]") {
    Tokenize t("    (   1              2     )");
    REQUIRE(!t.eof());
    REQUIRE(t.next() == "(");
    REQUIRE(t.next() == "1");
    REQUIRE(t.next() == "2");
    REQUIRE(t.next() == ")");
    REQUIRE(t.eof());
}

