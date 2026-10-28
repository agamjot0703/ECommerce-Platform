#pragma once

#include <exception>
#include <string>

// ------------------------------------------------------------
// Custom Exception Hierarchy for E-Commerce Platform
// Demonstrates OOP Exception Handling inheriting from std::exception
// ------------------------------------------------------------

// Base exception for platform errors
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

// Thrown when an order exceeds available inventory stock
class InsufficientStockException : public AppException
{
public:
    explicit InsufficientStockException(const std::string& productName)
        : AppException("Not enough stock available for product: " + productName) {}
};

// Thrown when customer wallet balance is insufficient for checkout
class InsufficientFundsException : public AppException
{
public:
    InsufficientFundsException(double available, double required)
        : AppException("Insufficient wallet balance!\nAvailable: $" +
                       std::to_string(available) + "\nRequired: $" + std::to_string(required)) {}
};

// Thrown when an unrecognized discount coupon is entered
class InvalidCouponException : public AppException
{
public:
    explicit InvalidCouponException(const std::string& code)
        : AppException("Coupon code \"" + code + "\" is invalid or expired.") {}
};
