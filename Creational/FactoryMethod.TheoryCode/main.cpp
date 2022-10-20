#include "factory.hpp"

#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace Canonical
{
    class Service
    {
        shared_ptr<LoggerCreator> creator_;
        bool is_started_ = false;

    public:
        Service(shared_ptr<LoggerCreator> creator)
            : creator_(creator)
        {
        }

        Service(const Service&) = delete;
        Service& operator=(const Service&) = delete;

        void run()
        {
            // creation of product
            unique_ptr<Logger> logger = creator_->create_logger();

            // usage of product
            if (!is_started_)
                start();
            logger->log("Service has been started...");
            process_requests();
            logger->log("Service has processed all requests...");
        }

    protected:
        virtual void start() { }
        virtual void process_requests() { }
    };

    // parametrized factory
    using LoggerFactory = std::unordered_map<std::string, shared_ptr<LoggerCreator>>;
}

// static factory
std::unique_ptr<Logger> create_logger(const std::string& id)
{
    if (id == "FileLogger")
    {
        return std::make_unique<FileLogger>("data.log");
    }
    else if (id == "ConsoleLogger")
    {
        return std::make_unique<ConsoleLogger>();
    }

    throw std::invalid_argument("Unknown id");
}

////////////////////////////////////////////
// Modern C++

using LoggerCreator = std::function<std::unique_ptr<Logger>()>;

class Service
{
    LoggerCreator creator_;
    bool is_started_ = false;

public:
    Service(LoggerCreator creator)
        : creator_(creator)
    {
    }

    Service(const Service&) = delete;
    Service& operator=(const Service&) = delete;

    void run()
    {
        // creation of product
        unique_ptr<Logger> logger = creator_();

        // usage of product
        if (!is_started_)
            start();
        logger->log("Service has been started...");
        process_requests();
        logger->log("Service has processed all requests...");
    }

protected:
    virtual void start() { }
    virtual void process_requests() { }
};

using LoggerFactory = std::unordered_map<std::string, LoggerCreator>;

int main()
{
    LoggerFactory logger_factory;
    logger_factory.emplace("ConsoleLogger", &std::make_unique<ConsoleLogger>);
    logger_factory.emplace("FileLogger", [] { return make_unique<FileLogger>("data.log"); });
    logger_factory.emplace("DbLogger", [] { return make_unique<DbLogger>("localhost:9999"); });

    std::string id_from_config = "DbLogger";
    Service srv(logger_factory.at(id_from_config));
    srv.run();
}
