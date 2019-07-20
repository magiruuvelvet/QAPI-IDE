#ifndef LOGGER_NAMED_THREAD_HPP
#define LOGGER_NAMED_THREAD_HPP

#include <fmt/format.h>

#include <thread>
#include <mutex>
#include <string_view>

#include <map>

/**
 * sub class of std::thread with string naming support
 *
 * can be used as object or directly
 *
 * named_thread("my_name", std::thread forward args...)
 *
 * name_thread t;
 * t.get_name();
 *
 * named_thread::get_name(get_id());
 *
 * this this used for logging to understand what happens
 * in what thread to reduce overall confusion during
 * development and debugging
 *
 */

class named_thread : public std::thread
{
public:
    using std::thread::thread;

    // default constructor
    template<typename... Arguments>
    named_thread(const std::string_view &name, Arguments... args)
        : std::thread(std::forward<Arguments>(args)...)
    {
        this->thread_name = name;

        std::lock_guard lg{mutex};
        pool.insert(std::pair{this->get_id(), name});
    }

    // string literal constructor
    template<typename... Arguments>
    named_thread(const char *name, Arguments... args)
        : std::thread(std::forward<Arguments>(args)...)
    {
        this->thread_name = std::string_view{name};

        std::lock_guard lg{mutex};
        pool.insert(std::pair{this->get_id(), name});
    }

    static void set_name(const std::thread::id &id, const std::string &name)
    {
        std::lock_guard lg{mutex};
        pool.insert(std::pair{id, name});
    }

    constexpr const auto &get_name() const
    { return this->thread_name; }

    static const std::string &get_name(const std::thread::id &id)
    {
        std::lock_guard lg{mutex};
        if (pool.count(id) == 0)
        {
            // if the given thread doesn't have a name calculate the hash of its id
            // and use that as name instead, this operation is only performed once
            // per unnamed thread
            const std::string name = fmt::format("0x{:X}", std::hash<std::thread::id>{}(id));
            pool.insert(std::pair{id, name});
            return pool.at(id);
        }
        else
        {
            return pool.at(id);
        }
    }

private:
    std::string_view thread_name;

    static std::mutex mutex;
    static std::map<std::thread::id, std::string> pool;
};

#endif // LOGGER_NAMED_THREAD_HPP
