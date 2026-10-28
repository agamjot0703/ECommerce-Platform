#include <QApplication>
#include "loginwindow.h"
#include "filemanager.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setApplicationName("E-Commerce Platform");

    FileManager::seedProductsIfMissing("inventory.txt");
    FileManager::seedUsersIfMissing("users.txt");

    LoginWindow loginWindow;
    loginWindow.setWindowTitle("E-Commerce Platform – Login");
    loginWindow.resize(400, 500);
    loginWindow.show();

    return app.exec();
}
