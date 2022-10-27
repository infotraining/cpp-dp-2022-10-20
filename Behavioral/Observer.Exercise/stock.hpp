#ifndef STOCK_HPP_
#define STOCK_HPP_

#include <boost/signals2.hpp>
#include <iostream>
#include <string>

using PriceChangedEvent = boost::signals2::signal<void(const std::string&, double)>;

// Subject
class Stock
{
private:
    std::string symbol_;
    double price_;

public:
    PriceChangedEvent price_changed;

    Stock(const std::string& symbol, double price)
        : symbol_(symbol)
        , price_(price)
    {
    }

    std::string get_symbol() const
    {
        return symbol_;
    }

    double get_price() const
    {
        return price_;
    }

    void set_price(double price)
    {
        if (price != price_)
        {
            price_ = price;

            price_changed(symbol_, price_);
        }
    }
};

class Investor 
{
    std::string name_;

public:
    Investor(const std::string& name)
        : name_(name)
    {
    }

    void update(const std::string& symbol, double price)
    {
        std::cout << name_ << " is notified on price change: " << symbol << " - " << price << "$\n";
    }
};

#endif /*STOCK_HPP_*/
