#pragma once

#include "user.h"
#include "product.h"
#include "order.h"
#include <QVector>

class Admin : public User
{
public:

    Admin(const QString& username,
          const QString& password,
          const QString& email);

    QString displayRole() const override;

    QString toString() const override;

    void addProduct(QVector<Product>& products, const Product& newProduct);

    bool updateProduct(QVector<Product>& products,
                       const QString& productId,
                       const Product& updatedProduct);

    void viewOrders(const QVector<Order>& orders) const;
};
