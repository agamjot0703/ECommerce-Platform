#include <QApplication>
#include "loginwindow.h"
#include "filemanager.h"

// ─────────────────────────────────────────────────────────────
// main.cpp  –  Application entry point
//
// Steps:
//  1. Create the QApplication object (required for every Qt app)
//  2. Seed data files on the very first run (no-op afterwards)
//  3. Show the LoginWindow
//  4. Hand control to the Qt event loop
// ─────────────────────────────────────────────────────────────
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Set a global application name visible in the title bars
    QApplication::setApplicationName("E-Commerce Platform");

    // ── First-run seeding ────────────────────────────────────
    // If inventory.txt does not exist, create it with sample products.
    // If users.txt does not exist, create it with a default admin
    // (username: admin / password: admin123) and a demo customer
    // (username: alice / password: alice123).
    FileManager::seedProductsIfMissing("inventory.txt");
    FileManager::seedUsersIfMissing("users.txt");

    // ── Show the login screen ────────────────────────────────
    LoginWindow loginWindow;
    loginWindow.setWindowTitle("E-Commerce Platform – Login");
    loginWindow.resize(400, 500);
    loginWindow.show();

    // ── Enter the Qt event loop ──────────────────────────────
    // Returns when the last window is closed.
    return app.exec();
}
