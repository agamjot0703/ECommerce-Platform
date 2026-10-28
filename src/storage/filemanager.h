#pragma once

#include <QString>
#include <QVector>
#include "product.h"
#include "order.h"

// Forward declarations to avoid circular includes
class User;

// ------------------------------------------------------------
// FileManager: Static utility class for all flat-file I/O.
// All methods are static – no instance of this class is needed.
//
// File paths are relative to QDir::currentPath() so the app
// works from whatever directory Qt Creator / the OS launches it.
// ------------------------------------------------------------
class FileManager
{
public:
    // --- Product I/O ---

    // Read every line of filename and return a vector of Products.
    // Returns an empty vector if the file does not exist.
    static QVector<Product> loadProducts(const QString& filename);

    // Overwrite filename with one product per line.
    static void saveProducts(const QString& filename,
                             const QVector<Product>& products);

    // --- Order I/O ---

    // Append a single order line to filename (creates file if needed).
    static void appendOrder(const QString& filename, const Order& order);

    // Read all order lines and return a vector of Orders.
    static QVector<Order> loadOrders(const QString& filename);

    // --- User I/O ---

    // Read users.txt and return heap-allocated User* objects.
    // Caller is responsible for deleting them.
    static QVector<User*> loadUsers(const QString& filename);

    // Overwrite filename with one user per line.
    static void saveUsers(const QString& filename,
                          const QVector<User*>& users);

    // --- Seed helpers ---

    // Create inventory.txt with sample products if it does not exist.
    static void seedProductsIfMissing(const QString& filename);

    // Create users.txt with a default admin account if it does not exist.
    static void seedUsersIfMissing(const QString& filename);

private:
    // Prevent instantiation – everything is static
    FileManager() = delete;
};
