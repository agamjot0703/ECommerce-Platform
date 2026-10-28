#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

// ------------------------------------------------------------
// LoginWindow: The first screen shown when the application starts.
//
// Responsibilities:
//  - Let existing users log in (validated against users.txt)
//  - Let new users register as Customer or Admin
//  - Open the appropriate dashboard on successful login
// ------------------------------------------------------------
class LoginWindow : public QWidget
{
    Q_OBJECT  // Required for signals/slots

public:
    // Constructs the window and builds all widgets
    explicit LoginWindow(QWidget* parent = nullptr);

private slots:
    // Called when the user clicks "Login"
    void onLoginClicked();

    // Called when the user clicks "Register"
    void onRegisterClicked();

private:
    // --- Widgets ---
    QLabel*       titleLabel;        // "E-Commerce Platform" heading
    QLineEdit*    usernameEdit;      // Login username field
    QLineEdit*    passwordEdit;      // Login password field (echo mode: Password)
    QPushButton*  loginButton;       // Triggers onLoginClicked()
    QPushButton*  registerButton;    // Triggers onRegisterClicked()

    // Registration section
    QGroupBox*    registerGroup;     // Groups register-specific widgets
    QLineEdit*    regUsernameEdit;   // New username
    QLineEdit*    regPasswordEdit;   // New password
    QLineEdit*    regEmailEdit;      // New email
    QRadioButton* customerRadio;     // Select "Customer" role
    QRadioButton* adminRadio;        // Select "Admin" role

    // --- Helpers ---
    void buildUI();                  // Create and arrange all widgets
    void showMessage(const QString& msg, bool isError = true);
};
