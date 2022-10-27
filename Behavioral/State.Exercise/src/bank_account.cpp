#include "bank_account.hpp"

Bank::NormalAccountState normal_state;
Bank::OverdraftAccountState overdraft_state;

Bank::IAccountState* Bank::IAccountState::normalState = &normal_state;
Bank::IAccountState* Bank::IAccountState::overdraftState = &overdraft_state;