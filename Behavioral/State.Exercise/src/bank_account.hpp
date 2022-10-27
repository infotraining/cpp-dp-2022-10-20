#ifndef BANK_ACCOUNT_HPP
#define BANK_ACCOUNT_HPP

#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace Bank
{
    class InsufficientFunds : public std::runtime_error
    {
        const int id_;

    public:
        InsufficientFunds(const std::string& msg, int id)
            : std::runtime_error{msg}
            , id_{id}
        {
        }

        int id() const
        {
            return id_;
        }
    };

    struct AccountContext
    {
        int id_;
        double balance_;
    };

    class IAccountState
    {
    public:
        ~IAccountState() = default;

        virtual void withdraw(AccountContext& context, double amount) = 0;
        virtual void pay_interest(AccountContext& context) = 0;
        virtual std::string status(const AccountContext& context) const = 0;

        static IAccountState* normalState;
        static IAccountState* overdraftState;
    };

    class NormalAccountState : public IAccountState
    {
    public:
        void withdraw(AccountContext& context, double amount) override
        {
            context.balance_ -= amount;
        }

        void pay_interest(AccountContext& context) override
        {
            context.balance_ += context.balance_ * 0.05;
        }

        std::string status(const AccountContext& context) const override
        {
            return "normal";
        }
    };

    class OverdraftAccountState : public IAccountState
    {
    public:
        void withdraw(AccountContext& context, double amount) override
        {
            throw InsufficientFunds{"Insufficient funds for account #" + std::to_string(context.id_), context.id_};
        }

        void pay_interest(AccountContext& context) override
        {
            context.balance_ += context.balance_ * 0.15;
        }

        std::string status(const AccountContext& context) const override
        {
            return "overdraft";
        }
    };

    class BankAccount
    {
        AccountContext context_;
        IAccountState* state_;

    protected:
        void update_account_state()
        {
            if (context_.balance_ < 0)
                state_ = IAccountState::overdraftState;
            else
                state_ = IAccountState::normalState;
        }

        void set_balance(double amount)
        {
            context_.balance_ = amount;
        }

    public:
        BankAccount(int id)
            : context_{id, 0.0}
            , state_(IAccountState::normalState)
        {
        }

        void withdraw(double amount)
        {
            assert(amount > 0);
            state_->withdraw(context_, amount);
            update_account_state();
        }

        void deposit(double amount)
        {
            assert(amount > 0);

            context_.balance_ += amount;

            update_account_state();
        }

        void pay_interest()
        {
            state_->pay_interest(context_);
        }

        std::string status() const
        {
            std::stringstream strm;
            strm << "BankAccount #" << context_.id_ << "; State: ";

            strm << state_->status(context_) << "; ";

            strm << "Balance: " << std::to_string(context_.balance_);

            return strm.str();
        }

        double balance() const
        {
            return context_.balance_;
        }

        int id() const
        {
            return context_.id_;
        }
    };
}

#endif
