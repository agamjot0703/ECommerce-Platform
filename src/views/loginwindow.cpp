#include "loginwindow.h"
#include "filemanager.h"
#include "customer.h"
#include "admin.h"
#include "customerdashboard.h"
#include "admindashboard.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QApplication>
#include <QLabel>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    buildUI();
}

void LoginWindow::buildUI()
{

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    titleLabel = new QLabel("🛒  E-Commerce Platform", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    loginButton    = new QPushButton("Login",    this);
    registerButton = new QPushButton("Register", this);

    registerGroup = new QGroupBox("Create a New Account", this);
    registerGroup->setVisible(false);

    regUsernameEdit = new QLineEdit(registerGroup);
    regUsernameEdit->setPlaceholderText("Choose a username");

    regPasswordEdit = new QLineEdit(registerGroup);
    regPasswordEdit->setPlaceholderText("Choose a password");
    regPasswordEdit->setEchoMode(QLineEdit::Password);

    regEmailEdit = new QLineEdit(registerGroup);
    regEmailEdit->setPlaceholderText("Email address");

    customerRadio = new QRadioButton("Customer", registerGroup);
    adminRadio    = new QRadioButton("Admin",    registerGroup);
    customerRadio->setChecked(true);

    QFormLayout *regForm = new QFormLayout(registerGroup);
    regForm->addRow("Username:", regUsernameEdit);
    regForm->addRow("Password:", regPasswordEdit);
    regForm->addRow("Email:",    regEmailEdit);

    QHBoxLayout *roleLayout = new QHBoxLayout();
    roleLayout->addWidget(customerRadio);
    roleLayout->addWidget(adminRadio);
    regForm->addRow("Register as:", roleLayout);

    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("Username:"));
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(new QLabel("Password:"));
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(registerGroup);
    mainLayout->addStretch();

    connect(loginButton,    &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);

    loginButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; "
        "padding: 8px; border-radius: 4px; font-size: 13px; }"
        "QPushButton:hover { background-color: #1976D2; }");

    registerButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "padding: 8px; border-radius: 4px; font-size: 13px; }"
        "QPushButton:hover { background-color: #388E3C; }");
}

void LoginWindow::onLoginClicked()
{
    QString enteredUser = usernameEdit->text().trimmed();
    QString enteredPass = passwordEdit->text();

    if (enteredUser.isEmpty() || enteredPass.isEmpty())
    {
        showMessage("Please enter both username and password.");
        return;
    }

    QVector<User *> users = FileManager::loadUsers("users.txt");

    User *loggedIn = nullptr;
    for (User *u : users)
    {
        if (u->getUsername() == enteredUser &&
            u->getPassword() == enteredPass)
        {
            loggedIn = u;
            break;
        }
    }

    if (loggedIn == nullptr)
    {
        showMessage("Invalid username or password.");

        for (User *u : users) delete u;
        return;
    }

    if (loggedIn->displayRole() == "Customer")
    {

        Customer *c = dynamic_cast<Customer *>(loggedIn);

        CustomerDashboard *dash = new CustomerDashboard(c);
        dash->setWindowTitle("Customer Dashboard – " + c->getUsername());
        dash->resize(800, 550);
        dash->show();

        for (User *u : users)
        {
            if (u != loggedIn) delete u;
        }
    }
    else if (loggedIn->displayRole() == "Admin")
    {
        Admin *a = dynamic_cast<Admin *>(loggedIn);

        AdminDashboard *dash = new AdminDashboard(a);
        dash->setWindowTitle("Admin Dashboard – " + a->getUsername());
        dash->resize(900, 600);
        dash->show();

        for (User *u : users)
        {
            if (u != loggedIn) delete u;
        }
    }

    this->hide();
}

void LoginWindow::onRegisterClicked()
{

    bool showing = registerGroup->isVisible();

    if (!showing)
    {

        registerGroup->setVisible(true);
        registerButton->setText("Create Account");
    }
    else
    {

        QString newUser  = regUsernameEdit->text().trimmed();
        QString newPass  = regPasswordEdit->text().trimmed();
        QString newEmail = regEmailEdit->text().trimmed();

        if (newUser.isEmpty() || newPass.isEmpty() || newEmail.isEmpty())
        {
            showMessage("Please fill in all registration fields.");
            return;
        }

        QVector<User *> existing = FileManager::loadUsers("users.txt");
        for (User *u : existing)
        {
            if (u->getUsername() == newUser)
            {
                showMessage("Username \"" + newUser + "\" is already taken.");
                for (User *x : existing) delete x;
                return;
            }
        }

        User *newUserObj = nullptr;
        if (customerRadio->isChecked())
            newUserObj = new Customer(newUser, newPass, newEmail);
        else
            newUserObj = new Admin(newUser, newPass, newEmail);

        existing.append(newUserObj);
        FileManager::saveUsers("users.txt", existing);

        for (User *u : existing) delete u;

        regUsernameEdit->clear();
        regPasswordEdit->clear();
        regEmailEdit->clear();
        registerGroup->setVisible(false);
        registerButton->setText("Register");

        QMessageBox::information(this, "Success",
            "Account created!\nYou can now log in as \"" + newUser + "\".");
    }
}

void LoginWindow::showMessage(const QString &msg, bool isError)
{
    if (isError)
        QMessageBox::warning(this, "Login", msg);
    else
        QMessageBox::information(this, "Login", msg);
}
