QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += multimedia
QT += webenginewidgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myrobot.cpp \
    robotcontroller.cpp

HEADERS += \
    mainwindow.h \
    myrobot.h \
    robotcontroller.h

FORMS += \
    mainwindow.ui \
    robotcontroller.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
