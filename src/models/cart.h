#pragma once

#include <QVector>
#include <QPair>
#include "product.h"

// ------------------------------------------------------------
// Cart: Holds the products a customer intends to purchase.
// Each entry is a (Product, quantity) pair.
// ------------------------------------------------------------
class Cart
{
public:
    Cart();  // Initialises an empty cart

    // Add a product to the cart.
    // If the product is already present, increase its quantity.
    void addItem(const Product& product, int quantity);

    // Remove the item at the given index (0-based) from the cart
    void removeItem(int index);

    // Calculate and return the total price of all items in the cart
    double calculateTotal() const;

    // Remove all items from the cart
    void clearCart();

    // Return a const reference to the internal item list
    const QVector<QPair<Product, int>>& getItems() const;

    // Return true if the cart has no items
    bool isEmpty() const;

    // Return the number of distinct items
    int itemCount() const;

    // Operator Overloading: shorthand to add an item pair to the cart
    Cart& operator+=(const QPair<Product, int>& item);

private:
    // Each element is {Product, quantity}
    QVector<QPair<Product, int>> items;
};
