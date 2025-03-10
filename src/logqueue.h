#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>

class LogQueue
{
  public:
    LogQueue() = default;
    ~LogQueue() = default;
    
    LogQueue(const LogQueue& queue)
    {
        log_queue = std::move(queue.log_queue);
    }
    void operator=(const LogQueue& queue)
    {
        log_queue = std::move(queue.log_queue);
    }

    void push(const std::string &log)
    {
        std::lock_guard<std::mutex> lock(mux);
        log_queue.push(log);
        cv.notify_one();
    }
    void pop()
    {
        std::unique_lock<std::mutex> lock(mux);
        cv.wait(lock, [this] { return !log_queue.empty(); });
        log_queue.pop();
    }
    bool empty()
    {
        std::lock_guard<std::mutex> lock(mux);
        return log_queue.empty();
    }
    std::string& front()
    {
        std::lock_guard<std::mutex> lock(mux);
        return log_queue.front();
    }
    std::string& back()
    {
        std::lock_guard<std::mutex> lock(mux);
        return log_queue.back();
    }
    size_t size()
    {
        std::lock_guard<std::mutex> lock(mux);
        return log_queue.size();
    }
  private:
    std::mutex mux;
    std::condition_variable cv;
    std::queue<std::string> log_queue;
};