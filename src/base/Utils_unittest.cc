#include "base/Utils.h"

#include <catch2/catch.hpp>

#include <iostream>

TEST_CASE("as_string") {
    std::string_view sv("hello");
    REQUIRE(muduo_rewrite::as_string(sv) == std::string("hello"));
}

TEST_CASE("readFile") {
    const auto content = muduo_rewrite::readFile("src/base/Utils.cc");
    REQUIRE(content.size() > 0);
}