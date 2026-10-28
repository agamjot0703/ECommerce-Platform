#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

class LoginWindow : public QWidget
{
    Q_OBJECT

public:

    explicit LoginWindow(QWidget* parent = nullptr);

private slots:

    void onLoginClicked();

    void onRegisterClicked();

private:

    QLabel*       titleLabel;
    QLineEdit*    usernameEdit;
    QLineEdit*    passwordEdit;
    QPushButton*  loginButton;
    QPushButton*  registerButton;

    QGroupBox*    registerGroup;
    QLineEdit*    regUsernameEdit;
    QLineEdit*    regPasswordEdit;
    QLineEdit*    regEmailEdit;
    QRadioButton* customerRadio;
    QRadioButton* adminRadio;

    void buildUI();
    void showMessage(const QString& msg, bool isError = true);
};
