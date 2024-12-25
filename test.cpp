#include "log.h"

int main()
{
    std::thread t1([](){
        for(int i = 0; i < 100; i++){
            Log::instance()->add("This is a debug message", DEBUG);
        }
    });
    std::thread t2([](){
        for(int i = 0; i < 100; i++){
            Log::instance()->add("This is a info message", INFO);
        }
    });
    std::thread t3([](){
        for(int i = 0; i < 100; i++){
            Log::instance()->add("This is a warning message", WARN);
        }
    });
    std::thread t4([](){
        for(int i = 0; i < 100; i++){
            Log::instance()->add("This is a error message", ERROR);
        }
    });
    t1.detach();
    t2.detach();
    t3.detach();
    t4.detach();
    std::this_thread::sleep_for(std::chrono::seconds(10));
    getchar();
    getchar();
    return 0;
}