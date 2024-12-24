#pragma once
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <chrono>

const std::string SAVING_PATH = "./log.txt";        // 日志保存路径
constexpr auto MAX_LOG_SIZE = 1024;                 // 日志最大缓冲数量
constexpr auto DEBUG = 0;
constexpr auto INFO = 1;
constexpr auto WARN = 2;
constexpr auto ERROR = 3;                           

class Log
{
public:
    inline static Log* instance(){
        if (log == nullptr)
        {
            std::lock_guard<std::mutex> lock(create_mux);
            if (log == nullptr) // Double-checked locking
                log = new Log();
        }
        return log;
    }

    void add(const std::string& message, int level = INFO){
        switch(level){
        case INFO:{
            std::lock_guard<std::mutex> lock(add_mux);
            std::string tmp = "[INFO]:" + message;
            log_queue.push(tmp);
            if(log_queue.size() > MAX_LOG_SIZE)
                flush();
            break;
        }
        case DEBUG:{
            std::lock_guard<std::mutex> lock(add_mux);
            std::string tmp = "[DEBUG]:" + message;
            log_queue.push(tmp);
            if(log_queue.size() > MAX_LOG_SIZE)
                flush();
            break;
        }
        case WARN:{
            std::lock_guard<std::mutex> lock(add_mux);
            std::string tmp = "[WARN]:" + message;
            log_queue.push(tmp);
            flush();
            break;
        }
        case ERROR:{
            std::lock_guard<std::mutex> lock(add_mux);
            std::string tmp = "[ERROR]:" + message;
            log_queue.push(tmp);
            std::cout<<tmp<<std::endl;
            flush();
            break;
        }
        default:
            break;
        }
    }

    void add(std::string&& message, int level = INFO){
        switch(level){
        case INFO:{
            std::lock_guard<std::mutex> lock(add_mux);
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto tm = std::localtime(&now);
            std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
            tmp += "[INFO]:" + message;
            log_queue.push(tmp);
            if(log_queue.size() > MAX_LOG_SIZE)
                flush();
            break;
        }
        case DEBUG:{
            std::lock_guard<std::mutex> lock(add_mux);
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto tm = std::localtime(&now);
            std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
            tmp += "[DEBUG]:" + message;
            log_queue.push(tmp);
            if(log_queue.size() > MAX_LOG_SIZE)
                flush();
            break;
        }
        case WARN:{
            std::lock_guard<std::mutex> lock(add_mux);
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto tm = std::localtime(&now);
            std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
            tmp += "[WARN]:" + message;
            log_queue.push(tmp);
            flush();
            break;
        }
        case ERROR:{
            std::lock_guard<std::mutex> lock(add_mux);
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto tm = std::localtime(&now);
            std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
            tmp += "[ERROR]:" + message;
            log_queue.push(tmp);
            std::cout<<tmp<<std::endl;
            flush();
            break;
        }
        default:
            break;
        }
    }

private:
    Log() = default;
    ~Log(){
        flush();
    }

    void flush(){
        auto f = [this](){
        std::lock_guard<std::mutex> lock(flush_mux);
        if(this->log_queue.empty()){
            return;
        }
        this->log_queue_tmp = std::move(this->log_queue);
        this->log_queue = std::queue<std::string>{};
        std::ofstream out(SAVING_PATH, std::ios::app);
        while(!(this->log_queue_tmp).empty()){
            out << (this->log_queue_tmp).front() << "\n";
            (this->log_queue_tmp).pop();
        }
        out.close();
        };
        std::thread t(f);
        t.detach();
    }
private:
    std::queue<std::string> log_queue;
    std::queue<std::string> log_queue_tmp;
    std::mutex flush_mux;
    std::mutex add_mux;
    inline static std::mutex create_mux{};
    inline static Log* log = nullptr;
};

#define LOG_INFO(message) Log::instance()->add(message, 1)
#define LOG_DEBUG(message) Log::instance()->add(message, 0)
#define LOG_WARN(message) Log::instance()->add(message, 2)
#define LOG_ERROR(message) Log::instance()->add(message, 3)