QT       += core gui printsupport sql charts network serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connexion.cpp \
    main.cpp \
    commande.cpp \
    smtp.cpp \
    materialswindow.cpp \
    arduinosensor.cpp \
    employe.cpp \
    logindialog.cpp

HEADERS += \
    connection.h \
    commande.h \
    smtp.h \
    materialswindow.h \
    animationutils.h \
    buttonanimation.h \
    arduinosensor.h \
    employe.h \
    logindialog.h

FORMS += \
    commande.ui \
    materialswindow.ui \
    employe.ui

RESOURCES += \
    resources.qrc

# Ensure IDE and compiler can find uic-generated headers in the build folder
INCLUDEPATH += $$PWD/build

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
