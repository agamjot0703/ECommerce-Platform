#include "admindashboard.h"
#include "filemanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QCloseEvent>
#include <QMessageBox>
#include <QStatusBar>
#include <QLabel>
#include <QDateTime>

AdminDashboard::AdminDashboard(Admin *admin, QWidget *parent)
    : QMainWindow(parent), admin(admin)
{
    buildUI();
    products = FileManager::loadProducts("inventory.txt");
    populateInventoryTable();
    populateOrdersTable();
    updateStats();

    statusBar()->showMessage(
        "Logged in as: " + admin->getUsername() +
        "  |  Role: " + admin->displayRole());
}

AdminDashboard::~AdminDashboard()
{
    delete admin;
}

void AdminDashboard::buildUI()
{
    QWidget     *central    = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(10);
    setCentralWidget(central);

    QLabel *titleLabel = new QLabel("⚙️  Admin Control Panel", central);
    QFont f = titleLabel->font();
    f.setPointSize(14);
    f.setBold(true);
    titleLabel->setFont(f);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    QHBoxLayout *statsLayout = new QHBoxLayout();

    statsRevenueLabel  = new QLabel("💰 Revenue: $0.00", central);
    statsOrdersLabel   = new QLabel("🧾 Orders: 0", central);
    statsProductsLabel = new QLabel("📦 Products: 0", central);

    QString cardStyle = "background-color: #E3F2FD; color: #0D47A1; padding: 8px; "
                        "border-radius: 4px; font-weight: bold; font-size: 12px;";

    statsRevenueLabel->setStyleSheet(cardStyle);
    statsOrdersLabel->setStyleSheet(cardStyle);
    statsProductsLabel->setStyleSheet(cardStyle);

    statsLayout->addWidget(statsRevenueLabel);
    statsLayout->addWidget(statsOrdersLabel);
    statsLayout->addWidget(statsProductsLabel);

    mainLayout->addLayout(statsLayout);

    tabWidget = new QTabWidget(central);

    QWidget *inventoryTab = new QWidget();
    QWidget *ordersTab    = new QWidget();

    buildInventoryTab(inventoryTab);
    buildOrdersTab(ordersTab);

    tabWidget->addTab(inventoryTab, "📦  Inventory Management");
    tabWidget->addTab(ordersTab,    "📋  Order History");

    mainLayout->addWidget(tabWidget);
}

void AdminDashboard::updateStats()
{
    QVector<Order> orders = FileManager::loadOrders("orders.txt");
    double totalRevenue = 0.0;
    for (const Order &o : orders)
    {
        totalRevenue += o.getTotalPrice();
    }

    int lowStockCount = 0;
    for (const Product &p : products)
    {
        if (p.getStock() <= 3) lowStockCount++;
    }

    statsRevenueLabel->setText("💰 Total Revenue: $" + QString::number(totalRevenue, 'f', 2));
    statsOrdersLabel->setText("🧾 Orders Completed: " + QString::number(orders.size()));

    QString prodStr = "📦 Total Products: " + QString::number(products.size());
    if (lowStockCount > 0)
        prodStr += " (" + QString::number(lowStockCount) + " Low Stock!)";
    statsProductsLabel->setText(prodStr);
}

void AdminDashboard::buildInventoryTab(QWidget *tab)
{
    QHBoxLayout *tabLayout = new QHBoxLayout(tab);
    tabLayout->setSpacing(12);

    inventoryTable = new QTableWidget(tab);
    inventoryTable->setColumnCount(5);
    inventoryTable->setHorizontalHeaderLabels(
        {"ID", "Name", "Price ($)", "Stock", "Category"});
    inventoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    inventoryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    inventoryTable->setSelectionMode(QAbstractItemView::SingleSelection);
    inventoryTable->horizontalHeader()->setStretchLastSection(true);
    inventoryTable->verticalHeader()->setVisible(false);
    inventoryTable->setAlternatingRowColors(true);
    inventoryTable->setMinimumWidth(380);

    connect(inventoryTable, &QTableWidget::cellClicked,
            this,           &AdminDashboard::onProductSelected);

    QGroupBox   *formBox    = new QGroupBox("Product Details", tab);
    QFormLayout *formLayout = new QFormLayout(formBox);
    formBox->setFixedWidth(280);

    idEdit       = new QLineEdit(formBox);
    nameEdit     = new QLineEdit(formBox);

    priceSpinBox = new QDoubleSpinBox(formBox);
    priceSpinBox->setRange(0.01, 99999.99);
    priceSpinBox->setDecimals(2);
    priceSpinBox->setPrefix("$ ");

    stockSpinBox = new QSpinBox(formBox);
    stockSpinBox->setRange(0, 99999);

    categoryEdit = new QLineEdit(formBox);

    formLayout->addRow("Product ID:",  idEdit);
    formLayout->addRow("Name:",        nameEdit);
    formLayout->addRow("Price:",       priceSpinBox);
    formLayout->addRow("Stock:",       stockSpinBox);
    formLayout->addRow("Category:",    categoryEdit);

    addProductButton       = new QPushButton("➕  Add Product",    formBox);
    updateProductButton    = new QPushButton("✏️  Update Product",  formBox);
    restockButton          = new QPushButton("⚡  Restock (+10)",   formBox);
    deleteProductButton    = new QPushButton("🗑️  Delete Product",  formBox);
    refreshInventoryButton = new QPushButton("🔄  Refresh",         formBox);

    addProductButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "padding: 6px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #388E3C; }");
    updateProductButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; "
        "padding: 6px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1976D2; }");
    restockButton->setStyleSheet(
        "QPushButton { background-color: #FF9800; color: white; "
        "padding: 6px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #F57C00; }");
    deleteProductButton->setStyleSheet(
        "QPushButton { background-color: #F44336; color: white; "
        "padding: 6px; border-radius: 4px; font-weight: bold; }"
        "QPushButton:hover { background-color: #D32F2F; }");
    refreshInventoryButton->setStyleSheet(
        "QPushButton { background-color: #9E9E9E; color: white; "
        "padding: 6px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #757575; }");

    QVBoxLayout *btnVBox = new QVBoxLayout();
    btnVBox->addWidget(addProductButton);
    btnVBox->addWidget(updateProductButton);
    btnVBox->addWidget(restockButton);
    btnVBox->addWidget(deleteProductButton);
    btnVBox->addWidget(refreshInventoryButton);
    btnVBox->addStretch();

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(formBox);
    rightLayout->addLayout(btnVBox);

    tabLayout->addWidget(inventoryTable, 1);
    tabLayout->addLayout(rightLayout);

    connect(addProductButton,       &QPushButton::clicked,
            this,                   &AdminDashboard::onAddProduct);
    connect(updateProductButton,    &QPushButton::clicked,
            this,                   &AdminDashboard::onUpdateProduct);
    connect(restockButton,          &QPushButton::clicked,
            this,                   &AdminDashboard::onQuickRestock);
    connect(deleteProductButton,    &QPushButton::clicked,
            this,                   &AdminDashboard::onDeleteProduct);
    connect(refreshInventoryButton, &QPushButton::clicked,
            this,                   &AdminDashboard::onRefreshInventory);
}

void AdminDashboard::buildOrdersTab(QWidget *tab)
{
    QVBoxLayout *tabLayout = new QVBoxLayout(tab);

    ordersTable = new QTableWidget(tab);
    ordersTable->setColumnCount(4);
    ordersTable->setHorizontalHeaderLabels(
        {"Order ID", "Customer", "Date", "Total ($)"});
    ordersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ordersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ordersTable->horizontalHeader()->setStretchLastSection(true);
    ordersTable->verticalHeader()->setVisible(false);
    ordersTable->setAlternatingRowColors(true);

    refreshOrdersButton = new QPushButton("🔄  Refresh Orders", tab);
    refreshOrdersButton->setStyleSheet(
        "QPushButton { background-color: #9E9E9E; color: white; "
        "padding: 7px; border-radius: 4px; }"
        "QPushButton:hover { background-color: #757575; }");

    tabLayout->addWidget(ordersTable);
    tabLayout->addWidget(refreshOrdersButton);

    connect(refreshOrdersButton, &QPushButton::clicked,
            this,                &AdminDashboard::onRefreshOrders);
}

void AdminDashboard::populateInventoryTable()
{
    inventoryTable->setRowCount(0);

    for (int i = 0; i < products.size(); ++i)
    {
        const Product &p = products[i];
        inventoryTable->insertRow(i);

        inventoryTable->setItem(i, 0, new QTableWidgetItem(p.getProductId()));
        inventoryTable->setItem(i, 1, new QTableWidgetItem(p.getName()));
        inventoryTable->setItem(i, 2, new QTableWidgetItem(
            QString::number(p.getPrice(), 'f', 2)));

        QString stockStr = QString::number(p.getStock());
        if (p.getStock() == 0) stockStr += " (Out of stock)";
        else if (p.getStock() <= 3) stockStr += " (Low stock)";

        QTableWidgetItem *stockItem = new QTableWidgetItem(stockStr);
        inventoryTable->setItem(i, 3, stockItem);
        inventoryTable->setItem(i, 4, new QTableWidgetItem(p.getCategory()));

        inventoryTable->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        stockItem->setTextAlignment(Qt::AlignCenter);

        if (p.getStock() == 0)
        {
            for (int col = 0; col < 5; ++col)
                inventoryTable->item(i, col)->setForeground(QBrush(QColor("#D32F2F")));
        }
        else if (p.getStock() <= 3)
        {
            for (int col = 0; col < 5; ++col)
                inventoryTable->item(i, col)->setForeground(QBrush(QColor("#E65100")));
        }
    }

    inventoryTable->resizeColumnsToContents();
}

void AdminDashboard::populateOrdersTable()
{
    ordersTable->setRowCount(0);

    QVector<Order> orders = FileManager::loadOrders("orders.txt");

    for (int i = 0; i < orders.size(); ++i)
    {
        const Order &o = orders[i];
        ordersTable->insertRow(i);

        ordersTable->setItem(i, 0, new QTableWidgetItem(o.getOrderId()));
        ordersTable->setItem(i, 1, new QTableWidgetItem(o.getCustomerUsername()));
        ordersTable->setItem(i, 2, new QTableWidgetItem(o.getOrderDate()));
        ordersTable->setItem(i, 3, new QTableWidgetItem(
            QString::number(o.getTotalPrice(), 'f', 2)));

        ordersTable->item(i, 3)->setTextAlignment(Qt::AlignCenter);
    }

    ordersTable->resizeColumnsToContents();
}

void AdminDashboard::onProductSelected()
{
    int row = inventoryTable->currentRow();
    if (row < 0 || row >= products.size()) return;

    const Product &p = products[row];
    idEdit->setText(p.getProductId());
    nameEdit->setText(p.getName());
    priceSpinBox->setValue(p.getPrice());
    stockSpinBox->setValue(p.getStock());
    categoryEdit->setText(p.getCategory());
}

void AdminDashboard::onAddProduct()
{
    QString id       = idEdit->text().trimmed();
    QString name     = nameEdit->text().trimmed();
    double  price    = priceSpinBox->value();
    int     stock    = stockSpinBox->value();
    QString category = categoryEdit->text().trimmed();

    if (id.isEmpty() || name.isEmpty() || category.isEmpty())
    {
        QMessageBox::warning(this, "Add Product",
                             "Product ID, Name, and Category cannot be empty.");
        return;
    }

    for (const Product &p : products)
    {
        if (p.getProductId() == id)
        {
            QMessageBox::warning(this, "Duplicate ID",
                "A product with ID \"" + id + "\" already exists.\n"
                "Use Update Product to modify it.");
            return;
        }
    }

    Product newProd(id, name, price, stock, category);
    admin->addProduct(products, newProd);

    populateInventoryTable();
    FileManager::saveProducts("inventory.txt", products);
    updateStats();
    statusBar()->showMessage("Product \"" + name + "\" added.", 3000);
}

void AdminDashboard::onUpdateProduct()
{
    QString id       = idEdit->text().trimmed();
    QString name     = nameEdit->text().trimmed();
    double  price    = priceSpinBox->value();
    int     stock    = stockSpinBox->value();
    QString category = categoryEdit->text().trimmed();

    if (id.isEmpty())
    {
        QMessageBox::warning(this, "Update Product",
                             "Select a product from the table first.");
        return;
    }

    Product updated(id, name, price, stock, category);
    bool ok = admin->updateProduct(products, id, updated);

    if (ok)
    {
        populateInventoryTable();
        FileManager::saveProducts("inventory.txt", products);
        updateStats();
        statusBar()->showMessage("Product \"" + id + "\" updated.", 3000);
    }
    else
    {
        QMessageBox::warning(this, "Not Found",
                             "No product with ID \"" + id + "\" found.");
    }
}

void AdminDashboard::onQuickRestock()
{
    int row = inventoryTable->currentRow();
    if (row < 0 || row >= products.size())
    {
        QMessageBox::warning(this, "Quick Restock", "Please select a product row to restock.");
        return;
    }

    Product &p = products[row];
    p.setStock(p.getStock() + 10);
    stockSpinBox->setValue(p.getStock());

    populateInventoryTable();
    FileManager::saveProducts("inventory.txt", products);
    updateStats();
    statusBar()->showMessage("Restocked +10 units for \"" + p.getName() + "\".", 3000);
}

void AdminDashboard::onDeleteProduct()
{
    int row = inventoryTable->currentRow();
    if (row < 0 || row >= products.size())
    {
        QMessageBox::warning(this, "Delete Product",
                             "Please select a product row to delete.");
        return;
    }

    QString name = products[row].getName();
    QMessageBox::StandardButton reply =
        QMessageBox::question(this, "Confirm Delete",
            "Delete \"" + name + "\" from inventory?",
            QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        products.removeAt(row);
        populateInventoryTable();
        FileManager::saveProducts("inventory.txt", products);
        updateStats();
        idEdit->clear();
        nameEdit->clear();
        categoryEdit->clear();
        statusBar()->showMessage("Product \"" + name + "\" deleted.", 3000);
    }
}

void AdminDashboard::onRefreshInventory()
{
    products = FileManager::loadProducts("inventory.txt");
    populateInventoryTable();
    updateStats();
    statusBar()->showMessage("Inventory refreshed.", 2000);
}

void AdminDashboard::onRefreshOrders()
{
    populateOrdersTable();
    updateStats();
    statusBar()->showMessage("Orders refreshed.", 2000);
}

void AdminDashboard::closeEvent(QCloseEvent *event)
{
    FileManager::saveProducts("inventory.txt", products);
    event->accept();
}
