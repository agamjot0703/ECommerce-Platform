#pragma once

#include <QString>

class Product
{
public:

    Product();

    Product(const QString& productId,
            const QString& name,
            double         price,
            int            stock,
            const QString& category);

    QString toString() const;

    static Product fromString(const QString& line);

    QString getProductId() const;
    QString getName()      const;
    double  getPrice()     const;
    int     getStock()     const;
    QString getCategory()  const;

    void setName(const QString& name);
    void setPrice(double price);
    void setStock(int stock);
    void setCategory(const QString& category);

    bool operator==(const Product& other) const;

private:
    QString productId;
    QString name;
    double  price;
    int     stock;
    QString category;
};
