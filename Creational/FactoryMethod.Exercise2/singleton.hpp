#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

template <typename T>
class SingletonHolder
{
private:
    SingletonHolder() = default;
    ~SingletonHolder() = default;

public:
    SingletonHolder(const SingletonHolder&) = delete;
    SingletonHolder& operator=(const SingletonHolder&) = delete;

    static T& instance()
    {
        static T unique_instance;

        return unique_instance;
    }
};

namespace OldVisualCompiler
{
    template <typename T>
    class SingletonHolder
    {
        inline static std::unique_ptr<T> instance_;
        inline static std::once_flag init_flag_;

    private:
        SingletonHolder() = default;
        ~SingletonHolder() = default;

    public:
        SingletonHolder(const SingletonHolder&) = delete;
        SingletonHolder& operator=(const SingletonHolder&) = delete;

        static T& instance()
        {
            std::call_once(init_flag_, []
                { instance_ = std::make_unique<T>(); });

            return *instance_;
        }
    };
}

namespace UnsafeSingleton
{
    template <typename T>
    class SingletonHolder
    {
        inline static std::unique_ptr<T> instance_;
        inline static std::mutex mtx_instance_;

    private:
        SingletonHolder() = default;
        ~SingletonHolder() = default;

    public:
        SingletonHolder(const SingletonHolder&) = delete;
        SingletonHolder& operator=(const SingletonHolder&) = delete;

        static T& instance()
        {
            std::lock_guard lk{mtx_instance_};

            if (!instance_)
                instance_ = std::make_unique<T>();

            return *instance_;
        }
    };

}

namespace Explain
{
    struct Logger
    {
        std::mutex mtx_;

        void log(const std::string& msg)
        {
            std::lock_guard lk{mtx_};
            std::cout << "Log: " << msg << " : at THD#" << std::this_thread::get_id() << "\n";
        }
    };

    using LoggerSingleton = SingletonHolder<Logger>;

    inline void run(Logger& logger)
    {
        logger.log("Hello");
    }

    inline void run_with_singleton()
    {
        LoggerSingleton::instance().log("Hello");
    }

    inline void singleton_and_thread_safety()
    {
        Logger logger;

        std::thread thd_1{[&]
            { run(logger); }};
        std::thread thd_2{[&]
            { run(logger); }};
        std::thread thd_3{[&]
            { run_with_singleton(); }};
        std::thread thd_4{[&]
            { run_with_singleton(); }};

        thd_1.join();
        thd_2.join();
        thd_3.join();
        thd_4.join();
    }
}

#endif // SINGLETON_HPP
