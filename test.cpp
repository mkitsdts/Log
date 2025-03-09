#include "log.h"

int main()
{
    std::thread t1([](){
        for(int i = 0; i < 1000; i++){
            Log::instance()->add("This is a debug message", DEBUG);
        }
    });
    std::thread t2([](){
        for(int i = 0; i < 1000; i++){
            Log::instance()->add("This is a info message", INFO);
        }
    });
    std::thread t3([](){
        for(int i = 0; i < 1000; i++){
            Log::instance()->add("This is a warning message", WARN);
        }
    });
    t1.detach();
    t2.detach();
    t3.detach();
    for(int i = 0; i < 100; i++){
        std::cout << i << std::endl;
    }
    return 0;
}