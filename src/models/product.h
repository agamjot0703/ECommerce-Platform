#pragma once

#include <QString>

// ------------------------------------------------------------
// Product: Represents a single item available in the store.
// Supports serialisation/deserialisation for flat-file storage.
// File format (one product per line):
//   ID|Name|Price|Stock|Category
// ------------------------------------------------------------
class Product
{
public:
    // Default constructor – creates an empty product
    Product();

    // Full constructor
    Product(const QString& productId,
            const QString& name,
            double         price,
            int            stock,
            const QString& category);

    // Serialise to pipe-delimited string for inventory.txt
    QString toString() const;

    // Static factory: parse a pipe-delimited line back into a Product
    static Product fromString(const QString& line);

    // --- Getters ---
    QString getProductId() const;
    QString getName()      const;
    double  getPrice()     const;
    int     getStock()     const;
    QString getCategory()  const;

    // --- Setters ---
    void setName(const QString& name);
    void setPrice(double price);
    void setStock(int stock);
    void setCategory(const QString& category);

    // Operator Overloading: compare products by ID
    bool operator==(const Product& other) const;

private:
    QString productId;   // Unique identifier, e.g. "P001"
    QString name;        // Display name
    double  price;       // Price in dollars
    int     stock;       // Units currently in stock
    QString category;    // e.g. "Electronics", "Books"
};
