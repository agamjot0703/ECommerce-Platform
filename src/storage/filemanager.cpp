#include "filemanager.h"
#include "user.h"
#include "customer.h"
#include "admin.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QCoreApplication>
#include <QDebug>

// Helper: build a full path checking data/ folder first, then current working directory
static QString fullPath(const QString& filename)
{
    // Check in data/ folder relative to current working directory
    QString dataPath = QDir::currentPath() + "/data/" + filename;
    if (QFile::exists(dataPath))
    {
        return dataPath;
    }

    // Check in data/ folder relative to executable directory (useful for Qt Creator builds)
    QString appDataPath = QCoreApplication::applicationDirPath() + "/data/" + filename;
    if (QFile::exists(appDataPath))
    {
        return appDataPath;
    }

    // If data/ directory exists, put new files there
    if (QDir(QDir::currentPath() + "/data").exists())
    {
        return dataPath;
    }

    return QDir::currentPath() + "/" + filename;
}

// ---------------------------------------------------------------
// loadProducts: read inventory.txt line by line
// ---------------------------------------------------------------
QVector<Product> FileManager::loadProducts(const QString& filename)
{
    QVector<Product> products;
    QFile file(fullPath(filename));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "FileManager::loadProducts – cannot open" << fullPath(filename);
        return products;   // Return empty vector on failure
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
        {
            Product p = Product::fromString(line);
            if (!p.getProductId().isEmpty())   // Skip malformed lines
            {
                products.append(p);
            }
        }
    }
    file.close();
    return products;
}

// ---------------------------------------------------------------
// saveProducts: overwrite inventory.txt with the current list
// ---------------------------------------------------------------
void FileManager::saveProducts(const QString& filename,
                               const QVector<Product>& products)
{
    QFile file(fullPath(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "FileManager::saveProducts – cannot open for writing:" << fullPath(filename);
        return;
    }

    QTextStream out(&file);
    for (const Product& p : products)
    {
        out << p.toString() << "\n";
    }
    file.close();
}

// ---------------------------------------------------------------
// appendOrder: add a single order line to orders.txt
// ---------------------------------------------------------------
void FileManager::appendOrder(const QString& filename, const Order& order)
{
    QFile file(fullPath(filename));
    if (!file.open(QIODevice::Append | QIODevice::Text))
    {
        qDebug() << "FileManager::appendOrder – cannot open for append:" << fullPath(filename);
        return;
    }

    QTextStream out(&file);
    out << order.toString() << "\n";
    file.close();
}

// ---------------------------------------------------------------
// loadOrders: read every order line from orders.txt
// ---------------------------------------------------------------
QVector<Order> FileManager::loadOrders(const QString& filename)
{
    QVector<Order> orders;
    QFile file(fullPath(filename));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "FileManager::loadOrders – cannot open" << fullPath(filename);
        return orders;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (!line.isEmpty())
        {
            Order o = Order::fromString(line);
            if (!o.getOrderId().isEmpty())
            {
                orders.append(o);
            }
        }
    }
    file.close();
    return orders;
}

// ---------------------------------------------------------------
// loadUsers: read users.txt and construct polymorphic User objects.
// The caller owns the returned pointers and must delete them.
// ---------------------------------------------------------------
QVector<User*> FileManager::loadUsers(const QString& filename)
{
    QVector<User*> users;
    QFile file(fullPath(filename));

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "FileManager::loadUsers – cannot open" << fullPath(filename);
        return users;
    }

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split("|");
        if (parts.size() < 4) continue;

        QString type     = parts[0].trimmed().toUpper();
        QString username = parts[1].trimmed();
        QString password = parts[2].trimmed();
        QString email    = parts[3].trimmed();

        if (type == "CUSTOMER")
        {
            double balance = 500.0;
            if (parts.size() >= 5)
            {
                balance = parts[4].trimmed().toDouble();
            }
            users.append(new Customer(username, password, email, balance));
        }
        else if (type == "ADMIN")
        {
            users.append(new Admin(username, password, email));
        }
        else
        {
            qDebug() << "FileManager::loadUsers – unknown type:" << type;
        }
    }
    file.close();
    return users;
}

// ---------------------------------------------------------------
// saveUsers: overwrite users.txt with the current user list
// ---------------------------------------------------------------
void FileManager::saveUsers(const QString& filename,
                            const QVector<User*>& users)
{
    QFile file(fullPath(filename));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "FileManager::saveUsers – cannot open for writing:" << fullPath(filename);
        return;
    }

    QTextStream out(&file);
    for (const User* u : users)
    {
        out << u->toString() << "\n";
    }
    file.close();
}

// ---------------------------------------------------------------
// seedProductsIfMissing: create a sample inventory on first run
// ---------------------------------------------------------------
void FileManager::seedProductsIfMissing(const QString& filename)
{
    QString path = fullPath(filename);
    if (QFile::exists(path)) return;   // Already present – nothing to do

    QVector<Product> samples = {
        Product("P001", "Laptop",          899.99, 10, "Electronics"),
        Product("P002", "Wireless Mouse",   29.99, 50, "Electronics"),
        Product("P003", "USB-C Hub",        49.99, 30, "Electronics"),
        Product("P004", "Desk Lamp",        19.99, 25, "Home"),
        Product("P005", "Python Book",      39.99, 15, "Books"),
        Product("P006", "Notebook (A4)",     4.99, 100,"Stationery"),
        Product("P007", "Coffee Mug",        9.99, 40, "Kitchen"),
        Product("P008", "Mechanical Keyboard",79.99,20,"Electronics")
    };

    saveProducts(filename, samples);
    qDebug() << "FileManager::seedProductsIfMissing – created" << path;
}

// ---------------------------------------------------------------
// seedUsersIfMissing: create a default admin + demo customer
// ---------------------------------------------------------------
void FileManager::seedUsersIfMissing(const QString& filename)
{
    QString path = fullPath(filename);
    if (QFile::exists(path)) return;

    QVector<User*> defaults;
    defaults.append(new Admin("admin", "admin123", "admin@store.com"));
    defaults.append(new Customer("alice", "alice123", "alice@example.com"));

    saveUsers(filename, defaults);

    // Clean up temporary objects
    for (User* u : defaults) delete u;

    qDebug() << "FileManager::seedUsersIfMissing – created" << path;
}
