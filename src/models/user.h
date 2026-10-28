#pragma once

#include <QString>

class User
{
public:

    virtual ~User();

    virtual QString displayRole() const = 0;

    virtual QString toString() const = 0;

    QString getUsername() const;
    QString getPassword() const;
    QString getEmail()    const;

    void setPassword(const QString& password);
    void setEmail(const QString& email);

protected:

    User(const QString& username,
         const QString& password,
         const QString& email);

    QString username;
    QString password;
    QString email;
};
