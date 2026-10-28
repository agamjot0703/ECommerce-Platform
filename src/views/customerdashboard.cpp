#include "customerdashboard.h"
#include "cartwindow.h"
#include "filemanager.h"
#include "order.h"
#include "exceptions.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QCloseEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QTextEdit>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QLabel>
#include <QStatusBar>

CustomerDashboard::CustomerDashboard(Customer *customer, QWidget *parent)
    : QMainWindow(parent), customer(customer)
{
    buildUI();
    loadAndDisplayProducts();
    updateBalanceDisplay();

    statusBar()->showMessage(
        "Logged in as: " + customer->getUsername() +
        "  |  Role: " + customer->displayRole());
}

CustomerDashboard::~CustomerDashboard()
{
    delete customer;
}

void CustomerDashboard::buildUI()
{
    QWidget     *central    = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    setCentralWidget(central);

    QHBoxLayout *topBarLayout = new QHBoxLayout();
    QLabel *titleLabel = new QLabel("🛍️  Customer Store", central);
    QFont f = titleLabel->font();
    f.setPointSize(14);
    f.setBold(true);
    titleLabel->setFont(f);

    balanceLabel = new QLabel(central);
    balanceLabel->setStyleSheet("font-size: 13px; font-weight: bold; color: #2E7D32;");

    addFundsButton = new QPushButton("➕ Add Funds", central);
    addFundsButton->setStyleSheet("background-color: #388E3C; color: white; padding: 5px 10px; border-radius: 4px;");

    myOrdersButton = new QPushButton("📜 My Orders", central);
    myOrdersButton->setStyleSheet("background-color: #5C6BC0; color: white; padding: 5px 10px; border-radius: 4px;");

    topBarLayout->addWidget(titleLabel);
    topBarLayout->addStretch();
    topBarLayout->addWidget(balanceLabel);
    topBarLayout->addWidget(addFundsButton);
    topBarLayout->addWidget(myOrdersButton);

    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *searchLabel = new QLabel("Search:", central);
    searchEdit = new QLineEdit(central);
    searchEdit->setPlaceholderText("Filter by product name or ID...");

    QLabel *catLabel = new QLabel("Category:", central);
    categoryCombo = new QComboBox(central);
    categoryCombo->addItem("All Categories");

    filterLayout->addWidget(searchLabel);
    filterLayout->addWidget(searchEdit, 2);
    filterLayout->addWidget(catLabel);
    filterLayout->addWidget(categoryCombo, 1);

    productTable = new QTableWidget(central);
    productTable->setColumnCount(5);
    productTable->setHorizontalHeaderLabels(
        {"ID", "Name", "Price ($)", "Stock", "Category"});
    productTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    productTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    productTable->setSelectionMode(QAbstractItemView::SingleSelection);
    productTable->horizontalHeader()->setStretchLastSection(true);
    productTable->verticalHeader()->setVisible(false);
    productTable->setAlternatingRowColors(true);

    QHBoxLayout *controlLayout = new QHBoxLayout();

    quantityLabel   = new QLabel("Quantity:", central);
    quantitySpinBox = new QSpinBox(central);
    quantitySpinBox->setMinimum(1);
    quantitySpinBox->setMaximum(999);
    quantitySpinBox->setValue(1);

    addToCartButton = new QPushButton("🛒  Add to Cart", central);
    viewCartButton  = new QPushButton("👀  View Cart",   central);
    refreshButton   = new QPushButton("🔄  Refresh",     central);

    controlLayout->addWidget(quantityLabel);
    controlLayout->addWidget(quantitySpinBox);
    controlLayout->addStretch();
    controlLayout->addWidget(addToCartButton);
    controlLayout->addWidget(viewCartButton);
    controlLayout->addWidget(refreshButton);

    mainLayout->addLayout(topBarLayout);
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(productTable);
    mainLayout->addLayout(controlLayout);

    addToCartButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; "
        "padding: 7px 14px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1976D2; }");

    viewCartButton->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; "
        "padding: 7px 14px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #F57C00; }");

    refreshButton->setStyleSheet(
        "QPushButton { background-color: #9E9E9E; color: white; "
        "padding: 7px 14px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #757575; }");

    connect(addToCartButton, &QPushButton::clicked,
            this,            &CustomerDashboard::onAddToCart);
    connect(viewCartButton,  &QPushButton::clicked,
            this,            &CustomerDashboard::onViewCart);
    connect(refreshButton,   &QPushButton::clicked,
            this,            &CustomerDashboard::onRefresh);
    connect(addFundsButton,  &QPushButton::clicked,
            this,            &CustomerDashboard::onAddFunds);
    connect(myOrdersButton,  &QPushButton::clicked,
            this,            &CustomerDashboard::onMyOrders);
    connect(searchEdit,      &QLineEdit::textChanged,
            this,            &CustomerDashboard::onFilterChanged);
    connect(categoryCombo,   &QComboBox::currentTextChanged,
            this,            &CustomerDashboard::onFilterChanged);
}

void CustomerDashboard::updateBalanceDisplay()
{
    balanceLabel->setText("💰 Wallet: $" + QString::number(customer->getBalance(), 'f', 2));
}

void CustomerDashboard::loadAndDisplayProducts()
{
    products = FileManager::loadProducts("inventory.txt");

    QString currentCat = categoryCombo->currentText();
    categoryCombo->blockSignals(true);
    categoryCombo->clear();
    categoryCombo->addItem("All Categories");

    QSet<QString> categories;
    for (const Product &p : products)
    {
        if (!p.getCategory().isEmpty())
            categories.insert(p.getCategory());
    }

    for (const QString &cat : categories)
    {
        categoryCombo->addItem(cat);
    }

    int idx = categoryCombo->findText(currentCat);
    if (idx >= 0) categoryCombo->setCurrentIndex(idx);
    categoryCombo->blockSignals(false);

    populateTable();
}

void CustomerDashboard::onFilterChanged()
{
    populateTable();
}

void CustomerDashboard::populateTable()
{
    productTable->setRowCount(0);

    QString query = searchEdit->text().trimmed();
    QString catFilter = categoryCombo->currentText();

    int row = 0;
    for (int i = 0; i < products.size(); ++i)
    {
        const Product &p = products[i];

        if (!query.isEmpty())
        {
            bool matchName = p.getName().contains(query, Qt::CaseInsensitive);
            bool matchId   = p.getProductId().contains(query, Qt::CaseInsensitive);
            if (!matchName && !matchId) continue;
        }

        if (catFilter != "All Categories" && p.getCategory() != catFilter)
        {
            continue;
        }

        productTable->insertRow(row);

        productTable->setItem(row, 0, new QTableWidgetItem(p.getProductId()));
        productTable->setItem(row, 1, new QTableWidgetItem(p.getName()));
        productTable->setItem(row, 2, new QTableWidgetItem(
            QString::number(p.getPrice(), 'f', 2)));

        QString stockStr = QString::number(p.getStock());
        if (p.getStock() == 0)
            stockStr += " (Out of stock)";
        else if (p.getStock() <= 3)
            stockStr += " (Low stock)";

        QTableWidgetItem *stockItem = new QTableWidgetItem(stockStr);
        productTable->setItem(row, 3, stockItem);
        productTable->setItem(row, 4, new QTableWidgetItem(p.getCategory()));

        productTable->item(row, 2)->setTextAlignment(Qt::AlignCenter);
        stockItem->setTextAlignment(Qt::AlignCenter);

        if (p.getStock() == 0)
        {
            for (int col = 0; col < 5; ++col)
            {
                productTable->item(row, col)->setForeground(QBrush(QColor("#D32F2F")));
            }
        }
        else if (p.getStock() <= 3)
        {
            for (int col = 0; col < 5; ++col)
            {
                productTable->item(row, col)->setForeground(QBrush(QColor("#E65100")));
            }
        }

        row++;
    }

    productTable->resizeColumnsToContents();
}

void CustomerDashboard::onAddToCart()
{
    int selectedRow = productTable->currentRow();
    if (selectedRow < 0)
    {
        QMessageBox::warning(this, "Add to Cart",
                             "Please select a product from the table first.");
        return;
    }

    QString selectedId = productTable->item(selectedRow, 0)->text();
    Product *selectedProduct = nullptr;
    for (Product &p : products)
    {
        if (p.getProductId() == selectedId)
        {
            selectedProduct = &p;
            break;
        }
    }

    if (!selectedProduct) return;

    int qty = quantitySpinBox->value();

    try
    {
        if (selectedProduct->getStock() == 0 || qty > selectedProduct->getStock())
        {
            throw InsufficientStockException(selectedProduct->getName().toStdString());
        }

        customer->getCart()->addItem(*selectedProduct, qty);

        statusBar()->showMessage(
            "Added " + QString::number(qty) + " × " +
            selectedProduct->getName() + " to cart.", 3000);
    }
    catch (const AppException &e)
    {
        QMessageBox::warning(this, "Stock Error", e.what());
    }
}

void CustomerDashboard::onViewCart()
{
    if (customer->getCart()->isEmpty())
    {
        QMessageBox::information(this, "Cart", "Your cart is currently empty.");
        return;
    }

    CartWindow cartWin(customer, products, this);
    cartWin.exec();

    updateBalanceDisplay();
    populateTable();
}

void CustomerDashboard::onAddFunds()
{
    bool ok = false;
    double amount = QInputDialog::getDouble(
        this, "Add Wallet Funds",
        "Enter amount to deposit into wallet ($):",
        50.0, 1.0, 10000.0, 2, &ok);

    if (ok && amount > 0.0)
    {
        customer->addBalance(amount);

        QVector<User *> allUsers = FileManager::loadUsers("users.txt");
        for (User *u : allUsers)
        {
            if (u->getUsername() == customer->getUsername())
            {
                Customer *c = dynamic_cast<Customer *>(u);
                if (c) c->addBalance(amount);
            }
        }
        FileManager::saveUsers("users.txt", allUsers);
        for (User *u : allUsers) delete u;

        updateBalanceDisplay();
        QMessageBox::information(this, "Deposit Successful",
            "Added $" + QString::number(amount, 'f', 2) + " to your wallet.\n"
            "New balance: $" + QString::number(customer->getBalance(), 'f', 2));
    }
}

void CustomerDashboard::onMyOrders()
{
    QVector<Order> allOrders = FileManager::loadOrders("orders.txt");
    QVector<Order> myOrders;

    for (const Order &o : allOrders)
    {
        if (o.getCustomerUsername() == customer->getUsername())
        {
            myOrders.append(o);
        }
    }

    if (myOrders.isEmpty())
    {
        QMessageBox::information(this, "My Orders", "You have not placed any orders yet.");
        return;
    }

    QDialog dlg(this);
    dlg.setWindowTitle("📜 My Order History");
    dlg.resize(500, 380);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QTextEdit *textEdit = new QTextEdit(&dlg);
    textEdit->setReadOnly(true);

    QString historyText;
    for (const Order &o : myOrders)
    {
        historyText += "-----------------------------------------\n";
        historyText += "Order ID : " + o.getOrderId() + "\n";
        historyText += "Date     : " + o.getOrderDate() + "\n";
        historyText += "Total    : $" + QString::number(o.getTotalPrice(), 'f', 2) + "\n";
        historyText += "Items:\n";
        for (const auto &item : o.getItems())
        {
            historyText += "  • " + item.first.getName() + " (x" +
                           QString::number(item.second) + ")\n";
        }
        historyText += "\n";
    }
    textEdit->setText(historyText);

    QPushButton *exportBtn = new QPushButton("📄 Export Latest Receipt (.txt)", &dlg);
    QPushButton *closeBtn  = new QPushButton("Close", &dlg);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addWidget(exportBtn);
    btnLayout->addWidget(closeBtn);

    layout->addWidget(textEdit);
    layout->addLayout(btnLayout);

    connect(closeBtn, &QPushButton::clicked, &dlg, &QDialog::close);
    connect(exportBtn, &QPushButton::clicked, [&]() {
        const Order &latest = myOrders.last();
        QString filename = "Receipt_" + latest.getOrderId() + ".txt";
        QFile file(filename);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << "=========================================\n";
            out << "         E-COMMERCE STORE RECEIPT        \n";
            out << "=========================================\n";
            out << "Order ID : " << latest.getOrderId() << "\n";
            out << "Customer : " << latest.getCustomerUsername() << "\n";
            out << "Date     : " << latest.getOrderDate() << "\n";
            out << "-----------------------------------------\n";
            out << "Purchased Items:\n";
            for (const auto &entry : latest.getItems())
            {
                out << "  - " << entry.first.getName()
                    << " x" << entry.second
                    << " @ $" << QString::number(entry.first.getPrice(), 'f', 2)
                    << "\n";
            }
            out << "-----------------------------------------\n";
            out << "Grand Total: $" << QString::number(latest.getTotalPrice(), 'f', 2) << "\n";
            out << "=========================================\n";
            out << "Thank you for your purchase!\n";
            file.close();

            QMessageBox::information(&dlg, "Exported",
                "Receipt exported successfully as: " + filename);
        }
    });

    dlg.exec();
}

void CustomerDashboard::onRefresh()
{
    loadAndDisplayProducts();
    updateBalanceDisplay();
    statusBar()->showMessage("Catalogue refreshed.", 2000);
}

void CustomerDashboard::closeEvent(QCloseEvent *event)
{
    FileManager::saveProducts("inventory.txt", products);
    event->accept();
}
