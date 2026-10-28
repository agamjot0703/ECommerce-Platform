#pragma once

#include "user.h"
#include "cart.h"
#include <QVector>
#include "product.h"

// ------------------------------------------------------------
// Customer: A registered shopper.
// Inherits from User and adds shopping-cart functionality.
// ------------------------------------------------------------
class Customer : public User
{
public:
    // Construct a Customer with login credentials and optional wallet balance
    Customer(const QString& username,
             const QString& password,
             const QString& email,
             double balance = 500.0);

    // Destructor cleans up the heap-allocated Cart
    ~Customer() override;

    // Returns "Customer" – identifies the role in the UI
    QString displayRole() const override;

    // Serialise to "CUSTOMER|username|password|email|balance" for users.txt
    QString toString() const override;

    // Wallet balance operations
    double getBalance() const;
    void   addBalance(double amount);
    bool   deductBalance(double amount);

    // Provide access to this customer's shopping cart
    Cart* getCart();

    // Browse available products.
    // In this GUI app the method prints to qDebug for demonstration;
    // the actual browsing happens in CustomerDashboard.
    void browseProducts(const QVector<Product>& products) const;

    // Place an order: reduces stock, records order, clears cart.
    // Returns true on success, false if cart is empty or stock is insufficient.
    bool placeOrder(QVector<Product>& products,
                    const QString& ordersFile,
                    const QString& productsFile);

private:
    Cart*  cart;      // Each customer owns exactly one cart
    double balance;   // Customer wallet balance in dollars
};
