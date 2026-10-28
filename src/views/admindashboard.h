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

class AdminDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminDashboard(Admin* admin, QWidget* parent = nullptr);
    ~AdminDashboard() override;

protected:

    void closeEvent(QCloseEvent* event) override;

private slots:

    void onAddProduct();
    void onUpdateProduct();
    void onDeleteProduct();
    void onQuickRestock();
    void onRefreshInventory();
    void onProductSelected();

    void onRefreshOrders();

private:
    Admin*          admin;
    QVector<Product> products;

    QTabWidget*     tabWidget;

    QLabel*         statsRevenueLabel;
    QLabel*         statsOrdersLabel;
    QLabel*         statsProductsLabel;

    QTableWidget*   inventoryTable;
    QPushButton*    addProductButton;
    QPushButton*    updateProductButton;
    QPushButton*    deleteProductButton;
    QPushButton*    restockButton;
    QPushButton*    refreshInventoryButton;

    QLineEdit*      idEdit;
    QLineEdit*      nameEdit;
    QDoubleSpinBox* priceSpinBox;
    QSpinBox*       stockSpinBox;
    QLineEdit*      categoryEdit;

    QTableWidget*   ordersTable;
    QPushButton*    refreshOrdersButton;

    void buildUI();
    void buildInventoryTab(QWidget* tab);
    void buildOrdersTab(QWidget* tab);
    void populateInventoryTable();
    void populateOrdersTable();
    void updateStats();
};
