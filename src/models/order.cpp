#include "order.h"
#include <QStringList>
#include <QDebug>

// Default constructor
Order::Order()
    : orderId(""), customerUsername(""), totalPrice(0.0), orderDate("")
{
}

// Full constructor – called at checkout time
Order::Order(const QString& orderId,
             const QString& customerUsername,
             const QVector<QPair<Product, int>>& items,
             double         totalPrice,
             const QString& orderDate)
    : orderId(orderId),
      customerUsername(customerUsername),
      items(items),
      totalPrice(totalPrice),
      orderDate(orderDate)
{
}

// ------------------------------------------------------------
// toString: serialise to a single line for orders.txt
// Format: OrderID|CustomerUsername|Date|TotalPrice|PID:Qty,PID:Qty,...
// ------------------------------------------------------------
QString Order::toString() const
{
    // Build the product-quantity segment, e.g. "P001:2,P003:1"
    QStringList productParts;
    for (const QPair<Product, int>& entry : items)
    {
        // Store enough info to reconstruct the order display:
        // PID:Name:Price:Qty   (colons inside fields avoided by design)
        QString segment = entry.first.getProductId()
                          + ":" + entry.first.getName()
                          + ":" + QString::number(entry.first.getPrice(), 'f', 2)
                          + ":" + QString::number(entry.second);
        productParts.append(segment);
    }

    return orderId + "|" +
           customerUsername + "|" +
           orderDate + "|" +
           QString::number(totalPrice, 'f', 2) + "|" +
           productParts.join(",");
}

// ------------------------------------------------------------
// fromString: parse a stored line back into an Order object.
// Format: OrderID|CustomerUsername|Date|TotalPrice|PID:Name:Price:Qty,...
// ------------------------------------------------------------
Order Order::fromString(const QString& line)
{
    QStringList parts = line.split("|");
    if (parts.size() < 5)
    {
        qDebug() << "Order::fromString – bad line:" << line;
        return Order();
    }

    QString orderId           = parts[0].trimmed();
    QString customerUsername  = parts[1].trimmed();
    QString orderDate         = parts[2].trimmed();
    double  totalPrice        = parts[3].trimmed().toDouble();

    // Parse the product list: "PID:Name:Price:Qty,PID:Name:Price:Qty"
    QVector<QPair<Product, int>> items;
    if (!parts[4].trimmed().isEmpty())
    {
        QStringList productTokens = parts[4].trimmed().split(",");
        for (const QString& token : productTokens)
        {
            QStringList seg = token.split(":");
            if (seg.size() >= 4)
            {
                QString pid   = seg[0].trimmed();
                QString name  = seg[1].trimmed();
                double  price = seg[2].trimmed().toDouble();
                int     qty   = seg[3].trimmed().toInt();

                // Reconstruct a lightweight Product (stock not needed for history)
                Product p(pid, name, price, 0, "");
                items.append(qMakePair(p, qty));
            }
        }
    }

    return Order(orderId, customerUsername, items, totalPrice, orderDate);
}

// --- Getters ---

QString Order::getOrderId()          const { return orderId;          }
QString Order::getCustomerUsername() const { return customerUsername; }
QString Order::getOrderDate()        const { return orderDate;        }
double  Order::getTotalPrice()       const { return totalPrice;       }

const QVector<QPair<Product, int>>& Order::getItems() const
{
    return items;
}
