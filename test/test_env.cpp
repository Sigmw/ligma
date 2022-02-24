#include "catch.hpp"
#include "env.hh"

TEST_CASE("Env basic set get", "[env]") {
    Env e;
    e.set("test", new StringValue("testValue"));

    Value *v = e.get("test");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::String );
    REQUIRE( toString(v)->value() == "testValue" );
}

TEST_CASE("Nested envs", "[env]") {
    Env e1;
    Env e2(&e1);

    e1.set("test", new StringValue("testValue"));
    e1.set("test2", new IntValue(42));

    Value *v = e2.get("test2");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::Int );
    REQUIRE( toInt(v)->value() == 42);

    e2.set("test2", new StringValue("testValue2"));
    v = e2.get("test2");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::String );
    REQUIRE( toString(v)->value() == "testValue2" );

    v = e1.get("test2");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::Int );
    REQUIRE( toInt(v)->value() == 42);

    v = e2.get("test");
    REQUIRE( v != nullptr );
    REQUIRE( v->type() == Type::String );
    REQUIRE( toString(v)->value() == "testValue" );
}
