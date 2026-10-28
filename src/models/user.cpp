#include "user.h"

User::User(const QString& username,
           const QString& password,
           const QString& email)
    : username(username), password(password), email(email)
{
}

User::~User()
{
}

QString User::getUsername() const
{
    return username;
}

QString User::getPassword() const
{
    return password;
}

QString User::getEmail() const
{
    return email;
}

void User::setPassword(const QString& password)
{
    this->password = password;
}

void User::setEmail(const QString& email)
{
    this->email = email;
}
