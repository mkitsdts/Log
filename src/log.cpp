#include "log.h"

inline LogQueue Log::log_queue;
inline LogQueue Log::log_queue_tmp;
inline std::mutex Log::mux;
inline std::condition_variable Log::cv;
inline std::unique_ptr<std::thread> Log::flush_thread;
inline std::mutex Log::create_mux;

inline Log log;