#include <type_traits>
#include <string>

#include <catch2/catch.hpp>

#include "absl/log/check.h"

#include "base/no_destructor.h"


namespace muduo_rewrite {

static_assert(!std::is_trivially_constructible_v<std::string>, "std::string is not trivially constructible");
static_assert(std::is_trivially_destructible_v<NoDestructor<std::string>>, "NoDestructor<std::string> is trivially destructible");

struct CheckOnDestroy {
    ~CheckOnDestroy() {
        CHECK(false) << "CheckOnDestroy is destructed";
    }
};

TEST_CASE("[SkipDestructor]", "NoDestructor") {
    NoDestructor<CheckOnDestroy> destructor_should_not_be_called;
}

struct UncopyableUnmovable {
    UncopyableUnmovable() = default;
    explicit UncopyableUnmovable(int val) : value(val) {}

    UncopyableUnmovable(const UncopyableUnmovable&) = delete;
    UncopyableUnmovable& operator=(const UncopyableUnmovable&) = delete;

    int value = 1;
    std::string something_with_a_non_trivial_destructor;
};

struct CopyOnly {
    CopyOnly() = default;

    CopyOnly(const CopyOnly&) = default;
    CopyOnly& operator=(const CopyOnly&) = default;

    CopyOnly(CopyOnly&&) = delete;
    CopyOnly& operator=(const CopyOnly&&) = delete;
};

struct MoveOnly {
    MoveOnly() = default;

    MoveOnly(const MoveOnly&) = delete;
    MoveOnly& operator=(const MoveOnly&) = delete;

    MoveOnly(MoveOnly&&) = default;
    MoveOnly& operator=(MoveOnly&&) = default;
};

struct ForwardingTestStruct {
    ForwardingTestStruct(const CopyOnly&, MoveOnly&&) {}

    std::string something_with_a_nontrivial_destructor;
};

TEST_CASE("[UncopyableUnmovable]", "NoDestructor") {
    static NoDestructor<UncopyableUnmovable> default_constructed;
    REQUIRE(default_constructed->value == 1);

    static NoDestructor<UncopyableUnmovable> value_constructed(42);
    REQUIRE(value_constructed->value == 42);
}

TEST_CASE("[ForwardsArguments]", "NoDestructor") {
    CopyOnly copy_only;
    MoveOnly move_only;

    static NoDestructor<ForwardingTestStruct> test_forwarding(copy_only, std::move(move_only));
}

TEST_CASE("[Accessors]", "NoDestructor") {
    static NoDestructor<std::string> s("Hello, world");

    REQUIRE(*s == "Hello, world");
    REQUIRE(s->size() == 12);
    REQUIRE(s.get()->compare("Hello, world") == 0);
}


}  // namespace muduo_rewrite
