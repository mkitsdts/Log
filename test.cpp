#include "src/log.h"

int main()
{
    std::thread t1([]() {
        for (int i = 0; i < 1000; i++)
        {
            Log::Info("This is a test message");
        }
    });
    std::thread t2([]() {
        for (int i = 0; i < 1000; i++)
        {
            Log::Warn("This is a test message");
        }
    });
    std::thread t3([]() {
        for (int i = 0; i < 1000; i++)
        {
            Log::Error("This is a test message");
        }
    });
    t1.detach();
    t2.detach();
    t3.detach();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}