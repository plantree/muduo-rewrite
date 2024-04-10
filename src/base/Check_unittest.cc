#include "absl/log/check.h"

int main() {
    CHECK(1 == 1) << "success";
    CHECK(1 == 2) << "failed";
}