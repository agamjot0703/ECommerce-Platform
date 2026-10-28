#pragma once

#include <QVector>
#include <QPair>
#include "product.h"

class Cart
{
public:
    Cart();

    void addItem(const Product& product, int quantity);

    void removeItem(int index);

    double calculateTotal() const;

    void clearCart();

    const QVector<QPair<Product, int>>& getItems() const;

    bool isEmpty() const;

    int itemCount() const;

    Cart& operator+=(const QPair<Product, int>& item);

private:

    QVector<QPair<Product, int>> items;
};
