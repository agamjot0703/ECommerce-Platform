#include "cartwindow.h"
#include "filemanager.h"
#include "order.h"
#include "exceptions.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDateTime>
#include <QLabel>
#include <QLineEdit>

// ─────────────────────────────────────────────────────────────
// Constructor: receive the customer, products list, and parent.
// ─────────────────────────────────────────────────────────────
CartWindow::CartWindow(Customer *customer,
                       QVector<Product> &products,
                       QWidget *parent)
    : QDialog(parent), customer(customer), products(products), discountPercent(0.0)
{
    setWindowTitle("🛒  Your Cart");
    setModal(true);
    resize(660, 460);

    buildUI();
    populateTable();
    updateTotalLabel();
}

// ─────────────────────────────────────────────────────────────
// buildUI: create table, coupon input, totals, and action buttons.
// ─────────────────────────────────────────────────────────────
void CartWindow::buildUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // ── Cart table ────────────────────────────────────────────
    cartTable = new QTableWidget(this);
    cartTable->setColumnCount(4);
    cartTable->setHorizontalHeaderLabels(
        {"Product Name", "Unit Price ($)", "Quantity", "Subtotal ($)"});
    cartTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    cartTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    cartTable->setSelectionMode(QAbstractItemView::SingleSelection);
    cartTable->horizontalHeader()->setStretchLastSection(true);
    cartTable->verticalHeader()->setVisible(false);
    cartTable->setAlternatingRowColors(true);

    // ── Coupon input row ──────────────────────────────────────
    QHBoxLayout *couponLayout = new QHBoxLayout();
    QLabel *couponLabel = new QLabel("Promo Code:", this);
    couponEdit = new QLineEdit(this);
    couponEdit->setPlaceholderText("Try 'SAVE10' or 'WELCOME20'");
    couponButton = new QPushButton("Apply Coupon", this);

    couponLayout->addWidget(couponLabel);
    couponLayout->addWidget(couponEdit);
    couponLayout->addWidget(couponButton);

    // ── Total and Balance labels ──────────────────────────────
    totalLabel = new QLabel("Total: $0.00", this);
    QFont f = totalLabel->font();
    f.setPointSize(11);
    f.setBold(true);
    totalLabel->setFont(f);
    totalLabel->setAlignment(Qt::AlignRight);

    balanceLabel = new QLabel("Wallet Balance: $0.00", this);
    balanceLabel->setStyleSheet("color: #2E7D32; font-weight: bold;");
    balanceLabel->setAlignment(Qt::AlignRight);

    // ── Buttons ───────────────────────────────────────────────
    removeButton   = new QPushButton("❌  Remove Item",  this);
    checkoutButton = new QPushButton("✅  Checkout",     this);
    closeButton    = new QPushButton("Close",            this);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(removeButton);
    btnLayout->addStretch();
    btnLayout->addWidget(checkoutButton);
    btnLayout->addWidget(closeButton);

    // ── Assemble ──────────────────────────────────────────────
    mainLayout->addWidget(cartTable);
    mainLayout->addLayout(couponLayout);
    mainLayout->addWidget(totalLabel);
    mainLayout->addWidget(balanceLabel);
    mainLayout->addLayout(btnLayout);

    // ── Styles ────────────────────────────────────────────────
    checkoutButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "padding: 7px 20px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #388E3C; }");

    removeButton->setStyleSheet(
        "QPushButton { background-color: #F44336; color: white; "
        "padding: 7px 14px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #D32F2F; }");

    couponButton->setStyleSheet(
        "QPushButton { background-color: #0288D1; color: white; "
        "padding: 6px 12px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #0277BD; }");

    // ── Connections ───────────────────────────────────────────
    connect(removeButton,   &QPushButton::clicked,
            this,           &CartWindow::onRemoveItem);
    connect(couponButton,   &QPushButton::clicked,
            this,           &CartWindow::onApplyCoupon);
    connect(checkoutButton, &QPushButton::clicked,
            this,           &CartWindow::onCheckout);
    connect(closeButton,    &QPushButton::clicked,
            this,           &QDialog::close);
}

// ─────────────────────────────────────────────────────────────
// populateTable: clear the table and re-fill it from the cart.
// ─────────────────────────────────────────────────────────────
void CartWindow::populateTable()
{
    cartTable->setRowCount(0);

    const QVector<QPair<Product, int>> &items = customer->getCart()->getItems();

    for (int i = 0; i < items.size(); ++i)
    {
        const Product &p   = items[i].first;
        int            qty = items[i].second;
        double subtotal    = p.getPrice() * qty;

        cartTable->insertRow(i);
        cartTable->setItem(i, 0, new QTableWidgetItem(p.getName()));
        cartTable->setItem(i, 1, new QTableWidgetItem(
            QString::number(p.getPrice(), 'f', 2)));
        cartTable->setItem(i, 2, new QTableWidgetItem(
            QString::number(qty)));
        cartTable->setItem(i, 3, new QTableWidgetItem(
            QString::number(subtotal, 'f', 2)));

        // Centre-align numbers
        for (int col = 1; col <= 3; ++col)
            cartTable->item(i, col)->setTextAlignment(Qt::AlignCenter);
    }

    cartTable->resizeColumnsToContents();
}

// ─────────────────────────────────────────────────────────────
// getFinalTotal: calculate grand total minus discount.
// ─────────────────────────────────────────────────────────────
double CartWindow::getFinalTotal() const
{
    double subtotal = customer->getCart()->calculateTotal();
    double discount = subtotal * discountPercent;
    double finalVal = subtotal - discount;
    return (finalVal > 0.0) ? finalVal : 0.0;
}

// ─────────────────────────────────────────────────────────────
// updateTotalLabel: recalculate and display breakdown.
// ─────────────────────────────────────────────────────────────
void CartWindow::updateTotalLabel()
{
    double subtotal = customer->getCart()->calculateTotal();
    double discount = subtotal * discountPercent;
    double finalTot = getFinalTotal();

    QString text = QString("Subtotal: $%1").arg(QString::number(subtotal, 'f', 2));
    if (discountPercent > 0.0)
    {
        text += QString("   |   Discount (%1%): -$%2   |   Final: $%3")
            .arg(QString::number(discountPercent * 100, 'f', 0))
            .arg(QString::number(discount, 'f', 2))
            .arg(QString::number(finalTot, 'f', 2));
    }
    else
    {
        text += QString("   |   Total: $%1").arg(QString::number(finalTot, 'f', 2));
    }

    totalLabel->setText(text);
    balanceLabel->setText("Available Wallet Balance: $" +
                          QString::number(customer->getBalance(), 'f', 2));
}

// ─────────────────────────────────────────────────────────────
// onApplyCoupon: validate promo code or throw custom exception.
// ─────────────────────────────────────────────────────────────
void CartWindow::onApplyCoupon()
{
    QString code = couponEdit->text().trimmed().toUpper();

    try
    {
        if (code == "SAVE10")
        {
            discountPercent = 0.10; // 10% discount
            QMessageBox::information(this, "Coupon Applied",
                "Coupon 'SAVE10' applied! You get 10% off.");
        }
        else if (code == "WELCOME20")
        {
            discountPercent = 0.20; // 20% discount
            QMessageBox::information(this, "Coupon Applied",
                "Coupon 'WELCOME20' applied! You get 20% off.");
        }
        else
        {
            throw InvalidCouponException(code.toStdString());
        }

        updateTotalLabel();
    }
    catch (const AppException &e)
    {
        discountPercent = 0.0;
        updateTotalLabel();
        QMessageBox::warning(this, "Coupon Error", e.what());
    }
}

// ─────────────────────────────────────────────────────────────
// onRemoveItem: remove the selected row from the cart.
// ─────────────────────────────────────────────────────────────
void CartWindow::onRemoveItem()
{
    int selectedRow = cartTable->currentRow();
    if (selectedRow < 0)
    {
        QMessageBox::warning(this, "Remove Item",
                             "Please select a row to remove.");
        return;
    }

    customer->getCart()->removeItem(selectedRow);
    populateTable();
    updateTotalLabel();
}

// ─────────────────────────────────────────────────────────────
// onCheckout: validate stock and wallet funds using custom
// exceptions, then record the order and update files.
// ─────────────────────────────────────────────────────────────
void CartWindow::onCheckout()
{
    try
    {
        if (customer->getCart()->isEmpty())
        {
            QMessageBox::information(this, "Checkout", "Your cart is empty.");
            return;
        }

        // ── 1. Stock validation ───────────────────────────────
        const QVector<QPair<Product, int>> &cartItems = customer->getCart()->getItems();

        for (const QPair<Product, int> &entry : cartItems)
        {
            bool found = false;
            for (const Product &p : products)
            {
                if (p.getProductId() == entry.first.getProductId())
                {
                    found = true;
                    if (p.getStock() < entry.second)
                    {
                        throw InsufficientStockException(p.getName().toStdString());
                    }
                    break;
                }
            }
            if (!found)
            {
                throw InsufficientStockException(entry.first.getName().toStdString());
            }
        }

        // ── 2. Wallet balance check ───────────────────────────
        double finalTotal = getFinalTotal();
        if (customer->getBalance() < finalTotal)
        {
            throw InsufficientFundsException(customer->getBalance(), finalTotal);
        }

        // ── 3. Deduct wallet funds ────────────────────────────
        customer->deductBalance(finalTotal);

        // Persist wallet update into users.txt
        QVector<User *> allUsers = FileManager::loadUsers("users.txt");
        for (User *u : allUsers)
        {
            if (u->getUsername() == customer->getUsername())
            {
                Customer *c = dynamic_cast<Customer *>(u);
                if (c) c->deductBalance(finalTotal);
            }
        }
        FileManager::saveUsers("users.txt", allUsers);
        for (User *u : allUsers) delete u;

        // ── 4. Build and save order ───────────────────────────
        QString orderId = "ORD-" + QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
        QString dateStr = QDate::currentDate().toString("yyyy-MM-dd");

        Order order(orderId,
                    customer->getUsername(),
                    cartItems,
                    finalTotal,
                    dateStr);

        FileManager::appendOrder("orders.txt", order);

        // ── 5. Deduct product stock ───────────────────────────
        for (const QPair<Product, int> &cartEntry : cartItems)
        {
            for (Product &p : products)
            {
                if (p.getProductId() == cartEntry.first.getProductId())
                {
                    p.setStock(p.getStock() - cartEntry.second);
                    break;
                }
            }
        }
        FileManager::saveProducts("inventory.txt", products);

        // ── 6. Clear cart and notify ──────────────────────────
        customer->getCart()->clearCart();

        QMessageBox::information(this, "Order Placed",
            "🎉 Order placed successfully!\n\n"
            "Order ID          : " + orderId + "\n"
            "Total Paid        : $" + QString::number(finalTotal, 'f', 2) + "\n"
            "Remaining Balance : $" + QString::number(customer->getBalance(), 'f', 2) + "\n\n"
            "Receipt saved to orders.txt!");

        populateTable();
        updateTotalLabel();
        close();
    }
    catch (const AppException &e)
    {
        QMessageBox::warning(this, "Checkout Failed", e.what());
    }
}
