#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>
#include "customer.h"
#include "product.h"

// ------------------------------------------------------------
// CustomerDashboard: Main window for a logged-in customer.
//
// Features:
//  - Shows the full product catalogue in a table
//  - Allows adding items to the cart with a chosen quantity
//  - Opens the CartWindow to review and checkout
//  - Saves updated inventory on close
// ------------------------------------------------------------
class CustomerDashboard : public QMainWindow
{
    Q_OBJECT

public:
    // Takes ownership of the Customer pointer passed from LoginWindow
    explicit CustomerDashboard(Customer* customer, QWidget* parent = nullptr);

    // Destructor: customer object is deleted here
    ~CustomerDashboard() override;

protected:
    // Save products back to inventory.txt before the window closes
    void closeEvent(QCloseEvent* event) override;

private slots:
    // Add the selected product (with chosen quantity) to the cart
    void onAddToCart();

    // Open the CartWindow dialog
    void onViewCart();

    // Re-read inventory.txt and refresh the table
    void onRefresh();

    // Live search and category filter slot
    void onFilterChanged();

    // Add funds to wallet balance
    void onAddFunds();

    // Export customer's past orders receipt
    void onMyOrders();

private:
    Customer*       customer;         // Logged-in customer (owned by this window)
    QVector<Product> products;        // In-memory copy of the inventory

    // --- Widgets ---
    QLineEdit*     searchEdit;        // Live name search
    QComboBox*     categoryCombo;     // Category dropdown filter
    QLabel*        balanceLabel;      // Shows wallet balance
    QPushButton*   addFundsButton;    // Add money to wallet
    QPushButton*   myOrdersButton;    // View/Export past receipts
    QTableWidget*  productTable;      // Displays the catalogue
    QPushButton*   addToCartButton;
    QPushButton*   viewCartButton;
    QPushButton*   refreshButton;
    QSpinBox*      quantitySpinBox;   // How many units to add
    QLabel*        quantityLabel;

    // --- Helpers ---
    void buildUI();
    void loadAndDisplayProducts();    // Reads inventory.txt, populates table
    void populateTable();             // Fills table from this->products
    void updateBalanceDisplay();      // Updates wallet display
};
