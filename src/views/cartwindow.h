#pragma once

#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "customer.h"
#include "product.h"

// ------------------------------------------------------------
// CartWindow: A modal dialog showing the customer's cart.
//
// Features:
//  - Displays each item, its unit price, quantity, and subtotal
//  - Shows the running grand total
//  - Allows removing individual items
//  - Checkout creates an Order, saves it to orders.txt,
//    deducts stock from inventory, and clears the cart
// ------------------------------------------------------------
class CartWindow : public QDialog
{
    Q_OBJECT

public:
    // products is passed by reference so stock can be decremented on checkout
    explicit CartWindow(Customer* customer,
                        QVector<Product>& products,
                        QWidget* parent = nullptr);

private slots:
    // Remove the currently selected row from the cart
    void onRemoveItem();

    // Finalise the purchase: create Order, save, clear cart
    void onCheckout();

    // Apply promo coupon code
    void onApplyCoupon();

private:
    Customer*        customer;    // The shopper (not owned – belongs to dashboard)
    QVector<Product>& products;  // Reference to dashboard's product list
    double           discountPercent; // Discount percent applied (e.g. 0.10 for 10%)

    // --- Widgets ---
    QTableWidget*  cartTable;    // One row per cart item
    QLineEdit*     couponEdit;   // Promo coupon code entry
    QPushButton*   couponButton; // Apply coupon
    QLabel*        totalLabel;   // Breakdown of subtotal, discount, total
    QLabel*        balanceLabel; // Customer wallet balance
    QPushButton*   removeButton;
    QPushButton*   checkoutButton;
    QPushButton*   closeButton;

    // --- Helpers ---
    void buildUI();
    void populateTable();        // Refresh cartTable from cart contents
    void updateTotalLabel();     // Recalculate and display total
    double getFinalTotal() const;// Total price after applying coupon
};
