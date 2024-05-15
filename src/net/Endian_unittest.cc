#include <catch2/catch.hpp>

#include "net/Endian.h"

TEST_CASE("Endian::hostToNetwork64", "[Endian]") {
  uint64_t host64 = 0x12345678;
  uint64_t net64 = muduo_rewrite::net::sockets::hostToNetwork64(host64);
  REQUIRE(net64 == 0x7856341200000000);
}

TEST_CASE("Endian::hostToNetwork32", "[Endian]") {
  uint32_t host32 = 0x12345678;
  uint32_t net32 = muduo_rewrite::net::sockets::hostToNetwork32(host32);
  REQUIRE(net32 == 0x78563412);
}

TEST_CASE("Endian::hostToNetwork16", "[Endian]") {
  uint16_t host16 = 0x1234;
  uint16_t net16 = muduo_rewrite::net::sockets::hostToNetwork16(host16);
  REQUIRE(net16 == 0x3412);
}

TEST_CASE("Endian::networkToHost64", "[Endian]") {
  uint64_t net64 = 0x7856341200000000;
  uint64_t host64 = muduo_rewrite::net::sockets::networkToHost64(net64);
  REQUIRE(host64 == 0x12345678);
}

TEST_CASE("Endian::networkToHost32", "[Endian]") {
  uint32_t net32 = 0x78563412;
  uint32_t host32 = muduo_rewrite::net::sockets::networkToHost32(net32);
  REQUIRE(host32 == 0x12345678);
}

TEST_CASE("Endian::networkToHost16", "[Endian]") {
  uint16_t net16 = 0x3412;
  uint16_t host16 = muduo_rewrite::net::sockets::networkToHost16(net16);
  REQUIRE(host16 == 0x1234);
}