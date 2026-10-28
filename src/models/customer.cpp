#include "customer.h"
#include "filemanager.h"
#include "order.h"
#include <QDateTime>
#include <QDebug>

Customer::Customer(const QString& username,
                   const QString& password,
                   const QString& email,
                   double balance)
    : User(username, password, email), balance(balance)
{
    cart = new Cart();
}

Customer::~Customer()
{
    delete cart;
    cart = nullptr;
}

QString Customer::displayRole() const
{
    return "Customer";
}

QString Customer::toString() const
{
    return "CUSTOMER|" + username + "|" + password + "|" + email + "|" + QString::number(balance, 'f', 2);
}

double Customer::getBalance() const
{
    return balance;
}

void Customer::addBalance(double amount)
{
    if (amount > 0.0)
    {
        balance += amount;
    }
}

bool Customer::deductBalance(double amount)
{
    if (amount > 0.0 && balance >= amount)
    {
        balance -= amount;
        return true;
    }
    return false;
}

Cart* Customer::getCart()
{
    return cart;
}

void Customer::browseProducts(const QVector<Product>& products) const
{
    qDebug() << "=== Available Products ===";
    for (const Product& p : products)
    {
        qDebug() << p.getProductId()
                 << p.getName()
                 << "$" + QString::number(p.getPrice(), 'f', 2)
                 << "Stock:" << p.getStock();
    }
}

bool Customer::placeOrder(QVector<Product>& products,
                          const QString& ordersFile,
                          const QString& productsFile)
{
    if (cart->getItems().isEmpty())
    {
        qDebug() << "placeOrder: cart is empty.";
        return false;
    }

    for (const QPair<Product, int>& entry : cart->getItems())
    {
        bool found = false;
        for (const Product& p : products)
        {
            if (p.getProductId() == entry.first.getProductId())
            {
                found = true;
                if (p.getStock() < entry.second)
                {
                    qDebug() << "Insufficient stock for" << p.getName();
                    return false;
                }
                break;
            }
        }
        if (!found)
        {
            qDebug() << "Product not found:" << entry.first.getProductId();
            return false;
        }
    }

    QString orderId  = "ORD-" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    QString dateStr  = QDate::currentDate().toString("yyyy-MM-dd");
    double  total    = cart->calculateTotal();

    Order order(orderId, username, cart->getItems(), total, dateStr);

    FileManager::appendOrder(ordersFile, order);

    for (const QPair<Product, int>& cartEntry : cart->getItems())
    {
        for (Product& p : products)
        {
            if (p.getProductId() == cartEntry.first.getProductId())
            {
                p.setStock(p.getStock() - cartEntry.second);
                break;
            }
        }
    }

    FileManager::saveProducts(productsFile, products);

    cart->clearCart();

    qDebug() << "Order placed:" << orderId << "Total: $" << total;
    return true;
}
