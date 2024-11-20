#include "log.h"
#include <ostream>
#include <iostream>

Log* Log::log = nullptr;

Log::Log():path("log.txt")
{
}

Log::~Log()
{
    flush();
}

Log* Log::instance()
{
    if (log == nullptr)
    {
        mux.lock();
        if (log == nullptr) // Double-checked locking
            log = new Log();
        mux.unlock();
    }
    return log;
}

void Log::write(std::string& message, int level)
{
    switch(level)
    {
    case INFO:
        std::string tmp = "[INFO]:"+message;
        log_queue.push(tmp);
        if(log_queue.size() > MAX_LOG_SIZE)
            flush();
        break;
    case DEBUG:
        std::string tmp = "[DEBUG]:"+message;
        log_queue.push(tmp);
        if(log_queue.size() > MAX_LOG_SIZE)
            flush();
        break;
    case WARN:
        std::string tmp = "[WARN]:"+message;
        log_queue.push(tmp);
        flush();
        break;
    case ERROR:
        std::string tmp = "[ERROR]:"+message;
        log_queue.push(tmp);
        std::cout<<tmp<<"\n";
        flush();
        break;
    default:
        break;
    }
}

void Log::flush()
{
    mux.lock();
    if(log_queue.empty())
    {
        mux.unlock();
        return;
    }
    log_queue_tmp = std::move(log_queue);
    log_queue = std::queue<std::string>();
    std::ofstream out(path, std::ios::app);
    while(!log_queue_tmp.empty())
    {
        out << log_queue_tmp.front() << "\n";
        log_queue_tmp.pop();
    }
    out.close(); 
    mux.unlock();
}