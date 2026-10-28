#include "product.h"
#include <QStringList>

Product::Product()
    : productId(""), name(""), price(0.0), stock(0), category("")
{
}

Product::Product(const QString& productId,
                 const QString& name,
                 double         price,
                 int            stock,
                 const QString& category)
    : productId(productId), name(name),
      price(price), stock(stock), category(category)
{
}

QString Product::toString() const
{
    return productId + "|" + name + "|" +
           QString::number(price, 'f', 2) + "|" +
           QString::number(stock) + "|" + category;
}

Product Product::fromString(const QString& line)
{
    QStringList parts = line.split("|");
    if (parts.size() < 5)
    {

        return Product();
    }

    QString id       = parts[0].trimmed();
    QString name     = parts[1].trimmed();
    double  price    = parts[2].trimmed().toDouble();
    int     stock    = parts[3].trimmed().toInt();
    QString category = parts[4].trimmed();

    return Product(id, name, price, stock, category);
}

QString Product::getProductId() const { return productId; }
QString Product::getName()      const { return name;      }
double  Product::getPrice()     const { return price;     }
int     Product::getStock()     const { return stock;     }
QString Product::getCategory()  const { return category;  }

void Product::setName(const QString& name)         { this->name = name;         }
void Product::setPrice(double price)               { this->price = price;       }
void Product::setStock(int stock)                  { this->stock = stock;       }
void Product::setCategory(const QString& category) { this->category = category; }

bool Product::operator==(const Product& other) const
{
    return this->productId == other.productId;
}
