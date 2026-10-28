QT += core gui widgets

CONFIG += c++17

TARGET = ECommercePlatform
TEMPLATE = app

INCLUDEPATH += \
    src \
    src/models \
    src/storage \
    src/views

SOURCES += \
    src/main.cpp \
    src/models/user.cpp \
    src/models/customer.cpp \
    src/models/admin.cpp \
    src/models/product.cpp \
    src/models/cart.cpp \
    src/models/order.cpp \
    src/storage/filemanager.cpp \
    src/views/loginwindow.cpp \
    src/views/customerdashboard.cpp \
    src/views/admindashboard.cpp \
    src/views/cartwindow.cpp

HEADERS += \
    src/models/exceptions.h \
    src/models/user.h \
    src/models/customer.h \
    src/models/admin.h \
    src/models/product.h \
    src/models/cart.h \
    src/models/order.h \
    src/storage/filemanager.h \
    src/views/loginwindow.h \
    src/views/customerdashboard.h \
    src/views/admindashboard.h \
    src/views/cartwindow.h
