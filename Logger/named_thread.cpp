#include <logger/named_thread.hpp>

std::mutex named_thread::mutex;
std::map<std::thread::id, std::string> named_thread::pool;
