#pragma once

#include <QString>

// ------------------------------------------------------------
// User: Abstract base class representing any platform user.
// Both Customer and Admin inherit from this class.
// Uses virtual methods so each subclass can define its own role.
// ------------------------------------------------------------
class User
{
public:
    // Virtual destructor – required when using polymorphism
    virtual ~User();

    // Pure-virtual: each subclass must return its role string
    virtual QString displayRole() const = 0;

    // Serialise this user to a single pipe-delimited line for users.txt
    // Format: Type|Username|Password|Email
    virtual QString toString() const = 0;

    // --- Getters ---
    QString getUsername() const;
    QString getPassword() const;
    QString getEmail()    const;

    // --- Setters ---
    void setPassword(const QString& password);
    void setEmail(const QString& email);

protected:
    // Protected constructor: only subclasses may instantiate
    User(const QString& username,
         const QString& password,
         const QString& email);

    QString username;   // Unique login name
    QString password;   // Plain-text password (acceptable for a lab project)
    QString email;      // Contact e-mail address
};
