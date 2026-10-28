#include "admin.h"
#include <QDebug>

// ------------------------------------------------------------
// Constructor: delegate to User base class
// ------------------------------------------------------------
Admin::Admin(const QString& username,
             const QString& password,
             const QString& email)
    : User(username, password, email)
{
}

// Role identifier
QString Admin::displayRole() const
{
    return "Admin";
}

// Serialise for users.txt: "ADMIN|username|password|email"
QString Admin::toString() const
{
    return "ADMIN|" + username + "|" + password + "|" + email;
}

// ------------------------------------------------------------
// addProduct: append a new product to the in-memory vector.
// The caller is responsible for persisting via FileManager.
// ------------------------------------------------------------
void Admin::addProduct(QVector<Product>& products, const Product& newProduct)
{
    products.append(newProduct);
    qDebug() << "Admin::addProduct –" << newProduct.getName() << "added.";
}

// ------------------------------------------------------------
// updateProduct: find by ID and replace the entry.
// Returns true if found, false otherwise.
// ------------------------------------------------------------
bool Admin::updateProduct(QVector<Product>& products,
                          const QString& productId,
                          const Product& updatedProduct)
{
    for (int i = 0; i < products.size(); ++i)
    {
        if (products[i].getProductId() == productId)
        {
            products[i] = updatedProduct;
            qDebug() << "Admin::updateProduct –" << productId << "updated.";
            return true;
        }
    }
    qDebug() << "Admin::updateProduct – product not found:" << productId;
    return false;
}

// ------------------------------------------------------------
// viewOrders: print orders to the debug console.
// Actual GUI display is handled by AdminDashboard.
// ------------------------------------------------------------
void Admin::viewOrders(const QVector<Order>& orders) const
{
    qDebug() << "=== All Orders ===";
    for (const Order& o : orders)
    {
        qDebug() << o.getOrderId()
                 << o.getCustomerUsername()
                 << o.getOrderDate()
                 << "$" + QString::number(o.getTotalPrice(), 'f', 2);
    }
}
