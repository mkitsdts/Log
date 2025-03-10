#include "log.h"

LogQueue Log::log_queue;
LogQueue Log::log_queue_tmp;
std::mutex Log::mux;
std::condition_variable Log::cv;
std::unique_ptr<std::thread> Log::flush_thread;
std::mutex Log::create_mux;

Log log;