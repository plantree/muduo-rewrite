#include "base/LogStream.h"

#include <catch2/catch.hpp>

TEST_CASE("FixedBuffer") {
    muduo_rewrite::detail::FixedBuffer<muduo_rewrite::detail::kSmallBuffer> buffer;
    REQUIRE(buffer.avail() == muduo_rewrite::detail::kSmallBuffer);
    REQUIRE(buffer.length() == 0);
    REQUIRE(buffer.data() == buffer.current());
    buffer.append("hello", 5);
    REQUIRE(buffer.avail() == muduo_rewrite::detail::kSmallBuffer - 5);

    buffer.add(2);
    REQUIRE(buffer.avail() == muduo_rewrite::detail::kSmallBuffer - 7);
    REQUIRE(buffer.length() == 7);

    buffer.reset();
    REQUIRE(buffer.avail() == muduo_rewrite::detail::kSmallBuffer);
    REQUIRE(buffer.length() == 0);

    buffer.bzero();
    for (int i = 0; i < muduo_rewrite::detail::kSmallBuffer; ++i) {
        REQUIRE(buffer.data()[i] == '\0');
    }
}