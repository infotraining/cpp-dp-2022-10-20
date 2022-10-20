#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <iostream>
#include <mutex>
#include <memory>

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
            std::call_once(init_flag_, []{ instance_ = std::make_unique<T>(); });

            return *instance_;
        }
    };
}

#endif // SINGLETON_HPP
