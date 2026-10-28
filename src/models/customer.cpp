#include "customer.h"
#include "filemanager.h"
#include "order.h"
#include <QDateTime>
#include <QDebug>

// ------------------------------------------------------------
// Constructor: initialise base class, then create the cart.
// ------------------------------------------------------------
Customer::Customer(const QString& username,
                   const QString& password,
                   const QString& email,
                   double balance)
    : User(username, password, email), balance(balance)
{
    cart = new Cart();   // Heap-allocate a fresh cart for this customer
}

// Destructor: release the cart
Customer::~Customer()
{
    delete cart;
    cart = nullptr;
}

// Role identifier used in the UI status bar
QString Customer::displayRole() const
{
    return "Customer";
}

// Serialise for users.txt: "CUSTOMER|username|password|email|balance"
QString Customer::toString() const
{
    return "CUSTOMER|" + username + "|" + password + "|" + email + "|" + QString::number(balance, 'f', 2);
}

// Return current wallet balance
double Customer::getBalance() const
{
    return balance;
}

// Add money to the wallet balance
void Customer::addBalance(double amount)
{
    if (amount > 0.0)
    {
        balance += amount;
    }
}

// Deduct money from the wallet balance
bool Customer::deductBalance(double amount)
{
    if (amount > 0.0 && balance >= amount)
    {
        balance -= amount;
        return true;
    }
    return false;
}

// Provide raw pointer access to the cart (not transferring ownership)
Cart* Customer::getCart()
{
    return cart;
}

// ------------------------------------------------------------
// browseProducts: debug helper – the real browsing UI is in
// CustomerDashboard.  This method exists to satisfy the OOP spec.
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// placeOrder: validate stock, build an Order, persist it,
// deduct stock from the in-memory vector, and clear the cart.
// Returns false if the cart is empty or any item lacks stock.
// ------------------------------------------------------------
bool Customer::placeOrder(QVector<Product>& products,
                          const QString& ordersFile,
                          const QString& productsFile)
{
    if (cart->getItems().isEmpty())
    {
        qDebug() << "placeOrder: cart is empty.";
        return false;
    }

    // Verify stock availability for every cart item
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

    // Build a unique order ID from the current timestamp
    QString orderId  = "ORD-" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
    QString dateStr  = QDate::currentDate().toString("yyyy-MM-dd");
    double  total    = cart->calculateTotal();

    Order order(orderId, username, cart->getItems(), total, dateStr);

    // Append order to orders.txt
    FileManager::appendOrder(ordersFile, order);

    // Deduct stock from the in-memory product list
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

    // Persist updated inventory
    FileManager::saveProducts(productsFile, products);

    // Empty the cart
    cart->clearCart();

    qDebug() << "Order placed:" << orderId << "Total: $" << total;
    return true;
}
