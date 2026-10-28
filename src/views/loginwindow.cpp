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

// ─────────────────────────────────────────────────────────────
// Constructor: Build all widgets and connect signals to slots.
// ─────────────────────────────────────────────────────────────
LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
{
    buildUI();
}

// ─────────────────────────────────────────────────────────────
// buildUI: create and lay out all widgets on this window.
// The window has two visual sections that toggle:
//   • Login section   – always visible
//   • Register group  – hidden by default, shown on Register click
// ─────────────────────────────────────────────────────────────
void LoginWindow::buildUI()
{
    // ── Main vertical layout ──────────────────────────────────
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);
    mainLayout->setContentsMargins(40, 30, 40, 30);

    // ── Title ─────────────────────────────────────────────────
    titleLabel = new QLabel("🛒  E-Commerce Platform", this);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);

    // ── Login form ────────────────────────────────────────────
    usernameEdit = new QLineEdit(this);
    usernameEdit->setPlaceholderText("Username");

    passwordEdit = new QLineEdit(this);
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);  // Mask the password

    loginButton    = new QPushButton("Login",    this);
    registerButton = new QPushButton("Register", this);

    // ── Registration group box ────────────────────────────────
    registerGroup = new QGroupBox("Create a New Account", this);
    registerGroup->setVisible(false);   // Hidden until user clicks "Register"

    regUsernameEdit = new QLineEdit(registerGroup);
    regUsernameEdit->setPlaceholderText("Choose a username");

    regPasswordEdit = new QLineEdit(registerGroup);
    regPasswordEdit->setPlaceholderText("Choose a password");
    regPasswordEdit->setEchoMode(QLineEdit::Password);

    regEmailEdit = new QLineEdit(registerGroup);
    regEmailEdit->setPlaceholderText("Email address");

    customerRadio = new QRadioButton("Customer", registerGroup);
    adminRadio    = new QRadioButton("Admin",    registerGroup);
    customerRadio->setChecked(true);   // Default selection

    // Lay out the registration fields
    QFormLayout *regForm = new QFormLayout(registerGroup);
    regForm->addRow("Username:", regUsernameEdit);
    regForm->addRow("Password:", regPasswordEdit);
    regForm->addRow("Email:",    regEmailEdit);

    // Role radio buttons in a horizontal sub-layout
    QHBoxLayout *roleLayout = new QHBoxLayout();
    roleLayout->addWidget(customerRadio);
    roleLayout->addWidget(adminRadio);
    regForm->addRow("Register as:", roleLayout);

    // ── Assemble the main layout ──────────────────────────────
    mainLayout->addWidget(titleLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(new QLabel("Username:"));
    mainLayout->addWidget(usernameEdit);
    mainLayout->addWidget(new QLabel("Password:"));
    mainLayout->addWidget(passwordEdit);
    mainLayout->addWidget(loginButton);
    mainLayout->addWidget(registerButton);
    mainLayout->addWidget(registerGroup);
    mainLayout->addStretch();    // Push everything to the top

    // ── Connect buttons to slots ──────────────────────────────
    connect(loginButton,    &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);

    // ── Basic stylesheet ──────────────────────────────────────
    loginButton->setStyleSheet(
        "QPushButton { background-color: #2196F3; color: white; "
        "padding: 8px; border-radius: 4px; font-size: 13px; }"
        "QPushButton:hover { background-color: #1976D2; }");

    registerButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "padding: 8px; border-radius: 4px; font-size: 13px; }"
        "QPushButton:hover { background-color: #388E3C; }");
}

// ─────────────────────────────────────────────────────────────
// onLoginClicked: validate credentials and open the right dashboard.
// ─────────────────────────────────────────────────────────────
void LoginWindow::onLoginClicked()
{
    QString enteredUser = usernameEdit->text().trimmed();
    QString enteredPass = passwordEdit->text();

    if (enteredUser.isEmpty() || enteredPass.isEmpty())
    {
        showMessage("Please enter both username and password.");
        return;
    }

    // Load all users from users.txt (polymorphic vector of User*)
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
        // Clean up all loaded user objects
        for (User *u : users) delete u;
        return;
    }

    // ── Determine role and open appropriate dashboard ─────────
    if (loggedIn->displayRole() == "Customer")
    {
        // Cast to Customer* so we can pass the full type to the dashboard
        Customer *c = dynamic_cast<Customer *>(loggedIn);

        // The dashboard takes ownership of the Customer object,
        // so we do NOT delete it here.
        CustomerDashboard *dash = new CustomerDashboard(c);
        dash->setWindowTitle("Customer Dashboard – " + c->getUsername());
        dash->resize(800, 550);
        dash->show();

        // Delete the other user objects (not the one we passed)
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

    // Hide (but keep) the login window so the user can log in again
    // after closing the dashboard (or close it entirely).
    this->hide();
}

// ─────────────────────────────────────────────────────────────
// onRegisterClicked: toggle the registration group box visibility.
// When the group box is showing, this button acts as "Create Account".
// ─────────────────────────────────────────────────────────────
void LoginWindow::onRegisterClicked()
{
    // Toggle the registration group visibility
    bool showing = registerGroup->isVisible();

    if (!showing)
    {
        // Show the registration section and change button label
        registerGroup->setVisible(true);
        registerButton->setText("Create Account");
    }
    else
    {
        // The user clicked "Create Account" – validate and save
        QString newUser  = regUsernameEdit->text().trimmed();
        QString newPass  = regPasswordEdit->text().trimmed();
        QString newEmail = regEmailEdit->text().trimmed();

        if (newUser.isEmpty() || newPass.isEmpty() || newEmail.isEmpty())
        {
            showMessage("Please fill in all registration fields.");
            return;
        }

        // Check if username already taken
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

        // Create the new user and append to users.txt
        User *newUserObj = nullptr;
        if (customerRadio->isChecked())
            newUserObj = new Customer(newUser, newPass, newEmail);
        else
            newUserObj = new Admin(newUser, newPass, newEmail);

        existing.append(newUserObj);
        FileManager::saveUsers("users.txt", existing);

        // Cleanup
        for (User *u : existing) delete u;

        // Reset form and hide the registration section
        regUsernameEdit->clear();
        regPasswordEdit->clear();
        regEmailEdit->clear();
        registerGroup->setVisible(false);
        registerButton->setText("Register");

        QMessageBox::information(this, "Success",
            "Account created!\nYou can now log in as \"" + newUser + "\".");
    }
}

// ─────────────────────────────────────────────────────────────
// showMessage: pop up an error/info dialog with the given message.
// ─────────────────────────────────────────────────────────────
void LoginWindow::showMessage(const QString &msg, bool isError)
{
    if (isError)
        QMessageBox::warning(this, "Login", msg);
    else
        QMessageBox::information(this, "Login", msg);
}
