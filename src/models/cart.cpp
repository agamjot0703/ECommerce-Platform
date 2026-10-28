#include "cart.h"

Cart::Cart()
{

}

void Cart::addItem(const Product& product, int quantity)
{

    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i].first.getProductId() == product.getProductId())
        {
            items[i].second += quantity;
            return;
        }
    }

    items.append(qMakePair(product, quantity));
}

void Cart::removeItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        items.removeAt(index);
    }
}

double Cart::calculateTotal() const
{
    double total = 0.0;
    for (const QPair<Product, int>& entry : items)
    {
        total += entry.first.getPrice() * entry.second;
    }
    return total;
}

void Cart::clearCart()
{
    items.clear();
}

const QVector<QPair<Product, int>>& Cart::getItems() const
{
    return items;
}

bool Cart::isEmpty() const
{
    return items.isEmpty();
}

int Cart::itemCount() const
{
    return items.size();
}

Cart& Cart::operator+=(const QPair<Product, int>& item)
{
    this->addItem(item.first, item.second);
    return *this;
}
