#ifndef COFFEEHELL_HPP_
#define COFFEEHELL_HPP_

#include <iostream>
#include <string>

class Coffee
{
public:
    virtual ~Coffee() = default;

    virtual float get_total_price() const = 0;
    virtual std::string get_description() const = 0;
    virtual void prepare() = 0;
};

class CoffeeBase : public Coffee
{
    float price_;
    std::string description_;

public:
    CoffeeBase(float price, const std::string& description) : price_{price}, description_{description}
    {
    }

    float get_total_price() const override
    {
        return price_;
    }

    std::string get_description() const override
    {
        return description_;
    }
};

class Espresso : public CoffeeBase
{
public:
    Espresso(float price = 4.0, const std::string& description = "Espresso")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect espresso: 7 g, 15 bar and 24 sec.\n";
    }
};

class Cappuccino : public CoffeeBase
{
public:
    Cappuccino(float price = 6.0, const std::string& description = "Cappuccino")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect cappuccino.\n";
    }
};

class Latte : public CoffeeBase
{
public:
    Latte(float price = 8.0, const std::string& description = "Latte")
        : CoffeeBase{price, description}
    {
    }

    void prepare() override
    {
        std::cout << "Making a perfect latte.\n";
    }
};

class Decorator : public Coffee
{
    std::unique_ptr<Coffee> coffee_;
public:
    Decorator(std::unique_ptr<Coffee> coffee)
        : coffee_(std::move(coffee))
    {}

    float get_total_price() const override
    {
        return coffee_->get_total_price();
    }

    std::string get_description() const override
    {
        return coffee_->get_description();
    }

    void prepare() override
    {
        coffee_->prepare();
    }
};

class DecoratorBase : public Decorator
{
    float price_;
    std::string description_;
public:
    DecoratorBase(std::unique_ptr<Coffee> coffee, float price, std::string description)
        : Decorator(std::move(coffee)), price_{price}, description_{std::move(description)}
    {}

    float get_total_price() const override
    {
        return Decorator::get_total_price() + price_;
    }

    std::string get_description() const override
    {
        return Decorator::get_description() + " " + description_;
    }
};

class Whipped : public DecoratorBase
{
public:
    Whipped(std::unique_ptr<Coffee> coffee)
        : DecoratorBase(std::move(coffee), 2.5, "with whipped cream")
    {}

    void prepare() override
    {
        DecoratorBase::prepare();
        std::cout << "Adding whipped cream\n";
    }
};

class Whisky : public DecoratorBase
{
public:
    Whisky(std::unique_ptr<Coffee> coffee)
        : DecoratorBase(std::move(coffee), 6.0, "with whisky")
    {}

    void prepare() override
    {
        DecoratorBase::prepare();
        std::cout << "Pouring 5cl of whisky\n";
    }
};

class ExtraEspresso : public Decorator
{
    Espresso espresso;
public:
    ExtraEspresso(std::unique_ptr<Coffee> coffee)
        : Decorator(std::move(coffee))
    {}

    float get_total_price() const override
    {
        return Decorator::get_total_price() + espresso.get_total_price();
    }

    std::string get_description() const override
    {
        return Decorator::get_description() + " with extra espresso";
    }

    void prepare() override
    {
        Decorator::prepare();
        Espresso espresso;
        espresso.prepare();
    }
};

// TO DO: Condiments: Whipped: 2.5$, Whisky: 6.0$, ExtraEspresso: 4.0$

// TO DO: Add CoffeeDecorator and concrete decorators for condiments 

#endif /*COFFEEHELL_HPP_*/
