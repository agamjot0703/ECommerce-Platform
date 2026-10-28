#include "admin.h"
#include <QDebug>

Admin::Admin(const QString& username,
             const QString& password,
             const QString& email)
    : User(username, password, email)
{
}

QString Admin::displayRole() const
{
    return "Admin";
}

QString Admin::toString() const
{
    return "ADMIN|" + username + "|" + password + "|" + email;
}

void Admin::addProduct(QVector<Product>& products, const Product& newProduct)
{
    products.append(newProduct);
    qDebug() << "Admin::addProduct –" << newProduct.getName() << "added.";
}

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
