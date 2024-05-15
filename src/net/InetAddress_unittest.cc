#include "net/InetAddress.h"

#include <catch2/catch.hpp>

TEST_CASE("InetAddress::basic", "[InetAddress]") {
  muduo_rewrite::net::InetAddress addr(80);
    REQUIRE(addr.family() == AF_INET);
    REQUIRE(addr.port() == 80);
    REQUIRE(addr.toIp() == "0.0.0.0");
}

TEST_CASE("InetAddress::resolve", "[InetAddress]") {
  muduo_rewrite::net::InetAddress addr(80);
  REQUIRE(muduo_rewrite::net::InetAddress::resolve("localhost", &addr));
  REQUIRE(addr.toIp() == "127.0.0.1");
  REQUIRE(addr.toIpPort() == "127.0.0.1:80");
}

TEST_CASE("InetAddress::endian", "[InetAddress]") {
    muduo_rewrite::net::InetAddress addr(80);
    REQUIRE(addr.ipv4NetEndian() == htonl(INADDR_ANY));
    REQUIRE(addr.port() == 80);
    REQUIRE(addr.toIpPort() == "0.0.0.0:80");
}

