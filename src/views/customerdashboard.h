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

class CustomerDashboard : public QMainWindow
{
    Q_OBJECT

public:

    explicit CustomerDashboard(Customer* customer, QWidget* parent = nullptr);

    ~CustomerDashboard() override;

protected:

    void closeEvent(QCloseEvent* event) override;

private slots:

    void onAddToCart();

    void onViewCart();

    void onRefresh();

    void onFilterChanged();

    void onAddFunds();

    void onMyOrders();

private:
    Customer*       customer;
    QVector<Product> products;

    QLineEdit*     searchEdit;
    QComboBox*     categoryCombo;
    QLabel*        balanceLabel;
    QPushButton*   addFundsButton;
    QPushButton*   myOrdersButton;
    QTableWidget*  productTable;
    QPushButton*   addToCartButton;
    QPushButton*   viewCartButton;
    QPushButton*   refreshButton;
    QSpinBox*      quantitySpinBox;
    QLabel*        quantityLabel;

    void buildUI();
    void loadAndDisplayProducts();
    void populateTable();
    void updateBalanceDisplay();
};
