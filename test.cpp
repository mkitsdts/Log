#include "log.h"
#include <chrono>

int main()
{
    auto log = Log::instance();
    log->add("Hello, world!", INFO);
    log->add("Hello, world!", DEBUG);
    log->add("Hello, world!", WARN);
    log->add("Hello, world!", ERROR);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    getchar();
    getchar();
    return 0;
}