#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <chrono>
#include <csignal>
#include <condition_variable>
#include "logqueue.h"

const std::string SAVING_PATH = "./log.txt"; // 日志保存路径

class Log
{
public:
    static void Debug(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mux);
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto tm = std::localtime(&now);
        std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
        tmp += "[DEBUG]:" + message;
        std::cout << tmp << std::endl;
    }

    static void Info(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mux);
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto tm = std::localtime(&now);
        std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
        tmp += "[INFO]:" + message;
        log_queue.push(tmp);
        cv.notify_one();
    }

    static void Warn(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mux);
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto tm = std::localtime(&now);
        std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
        tmp += "[WARN]:" + message;
        log_queue.push(tmp);
        cv.notify_one();
    }

    static void Error(const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mux);
        auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto tm = std::localtime(&now);
        std::string tmp = std::to_string(tm->tm_year + 1900) + "-" + std::to_string(tm->tm_mon + 1) + "-" + std::to_string(tm->tm_mday) + " " + std::to_string(tm->tm_hour) + ":" + std::to_string(tm->tm_min) + ":" + std::to_string(tm->tm_sec);
        tmp += "[ERROR]:" + message;
        log_queue.push(tmp);
        cv.notify_one();
    }

    Log()
    {
        flush_thread = std::make_unique<std::thread>([this]()
                                                     {
            while (true)
            {
                std::unique_lock<std::mutex> lock(mux);
                cv.wait(lock);
                std::cout << log_queue.size() << std::endl;
                if (this->log_queue.empty())
                {
                    continue;
                }
                this->log_queue_tmp = std::move(this->log_queue);
                this->log_queue = LogQueue{};
                std::ofstream out(SAVING_PATH, std::ios::app);
                while (!(this->log_queue_tmp).empty())
                {
                    out << (this->log_queue_tmp).front() << "\n";
                    (this->log_queue_tmp).pop();
                }
                out.close();
            } });
    }
    ~Log()
    {
        auto f = [this]()
        {
            std::lock_guard<std::mutex> lock(mux);
            if (this->log_queue.empty())
            {
                return;
            }
            std::ofstream out(SAVING_PATH, std::ios::app);
            while (!(this->log_queue).empty())
            {
                out << (this->log_queue_tmp).front() << "\n";
                (this->log_queue_tmp).pop();
            }
            out.close();
        };
        std::thread t(f);
        t.join();
    }

private:
    static LogQueue log_queue;
    static LogQueue log_queue_tmp;
    static std::mutex mux;
    static std::condition_variable cv;
    static std::unique_ptr<std::thread> flush_thread;
    static std::mutex create_mux;
};