#include "cart.h"

// Start with an empty item list
Cart::Cart()
{
    // items is default-constructed (empty QVector)
}

// ------------------------------------------------------------
// addItem: add a product to the cart.
// If the same product is already in the cart, just increase qty.
// ------------------------------------------------------------
void Cart::addItem(const Product& product, int quantity)
{
    // Search for existing entry with the same product ID
    for (int i = 0; i < items.size(); ++i)
    {
        if (items[i].first.getProductId() == product.getProductId())
        {
            items[i].second += quantity;   // Increase quantity
            return;
        }
    }

    // Product not yet in cart: add a new pair
    items.append(qMakePair(product, quantity));
}

// ------------------------------------------------------------
// removeItem: remove the entry at the given 0-based index.
// ------------------------------------------------------------
void Cart::removeItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        items.removeAt(index);
    }
}

// ------------------------------------------------------------
// calculateTotal: sum price * quantity for every cart entry.
// ------------------------------------------------------------
double Cart::calculateTotal() const
{
    double total = 0.0;
    for (const QPair<Product, int>& entry : items)
    {
        total += entry.first.getPrice() * entry.second;
    }
    return total;
}

// Remove everything from the cart
void Cart::clearCart()
{
    items.clear();
}

// Read-only access to the item list
const QVector<QPair<Product, int>>& Cart::getItems() const
{
    return items;
}

// True when the cart has no items
bool Cart::isEmpty() const
{
    return items.isEmpty();
}

// Number of distinct product lines in the cart
int Cart::itemCount() const
{
    return items.size();
}

// Overloaded += operator adds a product and quantity
Cart& Cart::operator+=(const QPair<Product, int>& item)
{
    this->addItem(item.first, item.second);
    return *this;
}
