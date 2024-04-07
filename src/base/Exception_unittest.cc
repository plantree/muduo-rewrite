#include "base/CurrentThread.h"
#include "base/Exception.h"

#include <functional>
#include <vector>

#include <stdio.h>

class Bar {
public:
    void test(std::vector<std::string> names) {
        printf("Stack:\n%s\n", muduo_rewrite::CurrentThread::stackTrace(true).c_str());
        []{
            printf("Stack inside lambda:\n%s\n", muduo_rewrite::CurrentThread::stackTrace(true).c_str());
        }();

        std::function<void()> func = []{
            printf("Stack inside std::function:\n%s\n", muduo_rewrite::CurrentThread::stackTrace(true).c_str());
        };
        func();

        func = std::bind(&Bar::callback, this);
        func();

        throw muduo_rewrite::Exception("oops");
    }

private:
    void callback() {
        printf("Stack inside callback:\n%s\n", muduo_rewrite::CurrentThread::stackTrace(true).c_str());
    }
};

void foo() {
    Bar bar;
    bar.test({"foo", "bar", "baz"});
}

int main() {
    try {
        foo();
    } catch (const muduo_rewrite::Exception& ex) {
        printf("reason: %s\n", ex.what());
        printf("stack trace: %s\n", ex.stackTrace());
    }
}