#include "user.h"

// ------------------------------------------------------------
// User constructor: initialises the three core fields.
// Protected – only called by subclass constructors.
// ------------------------------------------------------------
User::User(const QString& username,
           const QString& password,
           const QString& email)
    : username(username), password(password), email(email)
{
}

// Virtual destructor body (needed even though it's virtual)
User::~User()
{
}

// --- Getters ---

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

// --- Setters ---

void User::setPassword(const QString& password)
{
    this->password = password;
}

void User::setEmail(const QString& email)
{
    this->email = email;
}
