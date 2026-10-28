#pragma once

#include <QString>
#include <QVector>
#include "product.h"
#include "order.h"

class User;

class FileManager
{
public:

    static QVector<Product> loadProducts(const QString& filename);

    static void saveProducts(const QString& filename,
                             const QVector<Product>& products);

    static void appendOrder(const QString& filename, const Order& order);

    static QVector<Order> loadOrders(const QString& filename);

    static QVector<User*> loadUsers(const QString& filename);

    static void saveUsers(const QString& filename,
                          const QVector<User*>& users);

    static void seedProductsIfMissing(const QString& filename);

    static void seedUsersIfMissing(const QString& filename);

private:

    FileManager() = delete;
};
