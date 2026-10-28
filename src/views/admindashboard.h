#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVector>
#include "admin.h"
#include "product.h"
#include "order.h"

// ------------------------------------------------------------
// AdminDashboard: Main window for a logged-in administrator.
//
// Tab 1 – Inventory Management:
//   View, add, update, and delete products.
//
// Tab 2 – Order History:
//   Read-only view of all orders in orders.txt.
// ------------------------------------------------------------
class AdminDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminDashboard(Admin* admin, QWidget* parent = nullptr);
    ~AdminDashboard() override;

protected:
    // Save the inventory back to inventory.txt before closing
    void closeEvent(QCloseEvent* event) override;

private slots:
    // --- Inventory tab ---
    void onAddProduct();      // Read form fields and add a new product
    void onUpdateProduct();   // Update the product selected in the table
    void onDeleteProduct();   // Remove the selected product from the list
    void onQuickRestock();    // Quick restock +10 units to selected product
    void onRefreshInventory();// Reload inventory.txt
    void onProductSelected(); // Populate the form fields from the selected row

    // --- Orders tab ---
    void onRefreshOrders();   // Reload orders.txt and repopulate order table

private:
    Admin*          admin;        // Logged-in admin (owned here)
    QVector<Product> products;   // In-memory inventory

    // --- Tab widget ---
    QTabWidget*     tabWidget;

    // --- Analytics Summary Labels ---
    QLabel*         statsRevenueLabel;
    QLabel*         statsOrdersLabel;
    QLabel*         statsProductsLabel;

    // --- Inventory Tab widgets ---
    QTableWidget*   inventoryTable;
    QPushButton*    addProductButton;
    QPushButton*    updateProductButton;
    QPushButton*    deleteProductButton;
    QPushButton*    restockButton;
    QPushButton*    refreshInventoryButton;

    // Form fields for product data entry
    QLineEdit*      idEdit;
    QLineEdit*      nameEdit;
    QDoubleSpinBox* priceSpinBox;
    QSpinBox*       stockSpinBox;
    QLineEdit*      categoryEdit;

    // --- Orders Tab widgets ---
    QTableWidget*   ordersTable;
    QPushButton*    refreshOrdersButton;

    // --- Helpers ---
    void buildUI();
    void buildInventoryTab(QWidget* tab);
    void buildOrdersTab(QWidget* tab);
    void populateInventoryTable();
    void populateOrdersTable();
    void updateStats();
};
