#pragma once

#include "user.h"
#include "product.h"
#include "order.h"
#include <QVector>

// ------------------------------------------------------------
// Admin: A privileged user who manages the product inventory
// and can view all customer orders.
// ------------------------------------------------------------
class Admin : public User
{
public:
    // Construct an Admin with login credentials
    Admin(const QString& username,
          const QString& password,
          const QString& email);

    // Returns "Admin" – used in the UI status bar
    QString displayRole() const override;

    // Serialise to "ADMIN|username|password|email" for users.txt
    QString toString() const override;

    // Add a new product to the inventory vector
    void addProduct(QVector<Product>& products, const Product& newProduct);

    // Update an existing product identified by productId.
    // Returns true if the product was found and updated, false otherwise.
    bool updateProduct(QVector<Product>& products,
                       const QString& productId,
                       const Product& updatedProduct);

    // Print all orders to qDebug (GUI display is in AdminDashboard)
    void viewOrders(const QVector<Order>& orders) const;
};
