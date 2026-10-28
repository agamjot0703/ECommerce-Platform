#pragma once

#include "user.h"
#include "cart.h"
#include <QVector>
#include "product.h"

class Customer : public User
{
public:

    Customer(const QString& username,
             const QString& password,
             const QString& email,
             double balance = 500.0);

    ~Customer() override;

    QString displayRole() const override;

    QString toString() const override;

    double getBalance() const;
    void   addBalance(double amount);
    bool   deductBalance(double amount);

    Cart* getCart();

    void browseProducts(const QVector<Product>& products) const;

    bool placeOrder(QVector<Product>& products,
                    const QString& ordersFile,
                    const QString& productsFile);

private:
    Cart*  cart;
    double balance;
};
