#ifndef LOG_H
#define LOG_H

#include <queue>
#include <string>
#include <mutex>
#include <thread>

#define MAX_LOG_SIZE 1024;
#define INFO 0
#define DEBUG 1
#define WARN 2
#define ERROR 3

class Log
{
public:
    static Log* instance();
    void write(std::string& message, int level = 0);
    void write(std::string&& message, int level = 0);
    
private:
    Log();
    ~Log();
    void flush();
private:
    std::queue<std::string> log_queue;
    std::queue<std::string> log_queue_tmp;
    std::mutex mux;
    static Log* log;
    std::string path;
};

#define LOG_INFO(message) Log::instance()->write(message, 1)

#define LOG_DEBUG(message) Log::instance()->write(message, 0)

#define LOG_WARN(message) Log::instance()->write(message, 2)

#define LOG_ERROR(message) Log::instance()->write(message, 3)

#endif // LOG_H