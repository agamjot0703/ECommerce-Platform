#pragma once

#include <exception>
#include <string>

class AppException : public std::exception
{
protected:
    std::string message;

public:
    explicit AppException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override
    {
        return message.c_str();
    }
};

class InsufficientStockException : public AppException
{
public:
    explicit InsufficientStockException(const std::string& productName)
        : AppException("Not enough stock available for product: " + productName) {}
};

class InsufficientFundsException : public AppException
{
public:
    InsufficientFundsException(double available, double required)
        : AppException("Insufficient wallet balance!\nAvailable: $" +
                       std::to_string(available) + "\nRequired: $" + std::to_string(required)) {}
};

class InvalidCouponException : public AppException
{
public:
    explicit InvalidCouponException(const std::string& code)
        : AppException("Coupon code \"" + code + "\" is invalid or expired.") {}
};
