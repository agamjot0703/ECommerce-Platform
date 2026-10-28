#pragma once

#include <QString>
#include <QVector>
#include <QPair>
#include "product.h"

// ------------------------------------------------------------
// Order: A completed purchase snapshot.
// File format (one order per line in orders.txt):
//   OrderID|CustomerUsername|Date|TotalPrice|PID:Qty,PID:Qty,...
// ------------------------------------------------------------
class Order
{
public:
    // Default constructor
    Order();

    // Full constructor called when a customer checks out
    Order(const QString& orderId,
          const QString& customerUsername,
          const QVector<QPair<Product, int>>& items,
          double         totalPrice,
          const QString& orderDate);

    // Serialise to the single-line format stored in orders.txt
    QString toString() const;

    // Static factory: rebuild an Order from a stored line.
    // NOTE: product names/prices embedded in the line may be simplified;
    //       the loader reconstructs from the saved fields.
    static Order fromString(const QString& line);

    // --- Getters ---
    QString                          getOrderId()          const;
    QString                          getCustomerUsername() const;
    QString                          getOrderDate()        const;
    double                           getTotalPrice()       const;
    const QVector<QPair<Product,int>>& getItems()          const;

private:
    QString orderId;           // Unique order ID, e.g. "ORD-0001"
    QString customerUsername;  // Who placed the order
    QVector<QPair<Product, int>> items; // Snapshot of purchased products
    double  totalPrice;        // Grand total at checkout
    QString orderDate;         // ISO date string, e.g. "2024-01-15"
};
