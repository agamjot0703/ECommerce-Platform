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

class CartWindow : public QDialog
{
    Q_OBJECT

public:

    explicit CartWindow(Customer* customer,
                        QVector<Product>& products,
                        QWidget* parent = nullptr);

private slots:

    void onRemoveItem();

    void onCheckout();

    void onApplyCoupon();

private:
    Customer*        customer;
    QVector<Product>& products;
    double           discountPercent;

    QTableWidget*  cartTable;
    QLineEdit*     couponEdit;
    QPushButton*   couponButton;
    QLabel*        totalLabel;
    QLabel*        balanceLabel;
    QPushButton*   removeButton;
    QPushButton*   checkoutButton;
    QPushButton*   closeButton;

    void buildUI();
    void populateTable();
    void updateTotalLabel();
    double getFinalTotal() const;
};
