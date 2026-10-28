#pragma once

#include <QString>
#include <QVector>
#include <QPair>
#include "product.h"

class Order
{
public:

    Order();

    Order(const QString& orderId,
          const QString& customerUsername,
          const QVector<QPair<Product, int>>& items,
          double         totalPrice,
          const QString& orderDate);

    QString toString() const;

    static Order fromString(const QString& line);

    QString                          getOrderId()          const;
    QString                          getCustomerUsername() const;
    QString                          getOrderDate()        const;
    double                           getTotalPrice()       const;
    const QVector<QPair<Product,int>>& getItems()          const;

private:
    QString orderId;
    QString customerUsername;
    QVector<QPair<Product, int>> items;
    double  totalPrice;
    QString orderDate;
};
