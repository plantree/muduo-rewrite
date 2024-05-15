#include "net/SocketsOps.h"

#include <catch2/catch.hpp>
#include <cstring>

TEST_CASE("sockets::createNonBlockingOrDie", "[sockets]") {
  int sockfd = muduo_rewrite::net::sockets::createNonBlockingOrDie(AF_INET);
  REQUIRE(sockfd > 0);
}

TEST_CASE("sockets::sockaddr_cast6", "[sockets]") {
  struct sockaddr_in6 addr6;
  const struct sockaddr* addr = muduo_rewrite::net::sockets::sockaddr_cast(&addr6);
  REQUIRE(addr != nullptr);
}

TEST_CASE("sockets::sockaddr_cast4", "[sockets]") {
  struct sockaddr_in addr4;
  const struct sockaddr* addr = muduo_rewrite::net::sockets::sockaddr_cast(&addr4);
  REQUIRE(addr != nullptr);
}

TEST_CASE("sockets::sockaddr_in_cast4", "[sockets]") {
  struct sockaddr_in addr4;
  const struct sockaddr* addr = muduo_rewrite::net::sockets::sockaddr_cast(&addr4);
  REQUIRE(addr != nullptr);
  const struct sockaddr_in* addr4_ = muduo_rewrite::net::sockets::sockaddr_in_cast(addr);
  REQUIRE(addr4_ != nullptr);
  REQUIRE(addr4_->sin_addr.s_addr == addr4.sin_addr.s_addr);
}

TEST_CASE("sockets::sockaddr_in_cast6", "[sockets]") {
    struct sockaddr_in6 addr6;
    const struct sockaddr* addr = muduo_rewrite::net::sockets::sockaddr_cast(&addr6);
    REQUIRE(addr != nullptr);
    const struct sockaddr_in6* addr6_ = muduo_rewrite::net::sockets::sockaddr_in6_cast(addr);
    REQUIRE(addr6_ != nullptr);
    REQUIRE(addr6_->sin6_scope_id == addr6.sin6_scope_id);
}

TEST_CASE("sockets::fromIpPort4", "[sockets]") {
    struct sockaddr_in addr4;
    muduo_rewrite::net::sockets::fromIpPort("127.0.0.1", 1234, &addr4);
    REQUIRE(addr4.sin_family == AF_INET);
    REQUIRE(addr4.sin_port == htons(1234));
    REQUIRE(addr4.sin_addr.s_addr == htonl(0x7F000001));
}

TEST_CASE("sockets::fromIpPort6", "[sockets]") {
    struct sockaddr_in6 addr6;
    muduo_rewrite::net::sockets::fromIpPort("::1", 1234, &addr6);
    REQUIRE(addr6.sin6_family == AF_INET6);
    REQUIRE(addr6.sin6_port == htons(1234));
    REQUIRE(addr6.sin6_addr.s6_addr[15] == 1);
}

TEST_CASE("sockets::getLocalAddr", "[sockets]") {
    int sockfd = muduo_rewrite::net::sockets::createNonBlockingOrDie(AF_INET6);
    struct sockaddr_in6 addr = muduo_rewrite::net::sockets::getLocalAddr(sockfd);
    REQUIRE(addr.sin6_family == AF_INET6);
    muduo_rewrite::net::sockets::close(sockfd);
}

TEST_CASE("sockets::toIp", "[sockets]") {
    struct sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_addr.s_addr = htonl(0x7F000002);
    char buf[64] = "";
    muduo_rewrite::net::sockets::toIp(buf, sizeof buf, reinterpret_cast<const struct sockaddr*>(&addr4));
    REQUIRE(strcmp(buf, "127.0.0.2") == 0);
}

TEST_CASE("sockets::toIpPort", "[sockets]") {
    struct sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_addr.s_addr = htonl(0x7F000002);
    addr4.sin_port = htons(1234);
    char buf[64] = "";
    muduo_rewrite::net::sockets::toIpPort(buf, sizeof buf, reinterpret_cast<const struct sockaddr*>(&addr4));
    REQUIRE(strcmp(buf, "127.0.0.2:1234") == 0);
}








