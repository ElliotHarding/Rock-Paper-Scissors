QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    collisioncombobox.cpp \
    dlg_gamefeild.cpp \
    gameobject.cpp \
    main.cpp \
    mainwindow.cpp \
    wdg_gameobjectsettingsrow.cpp

HEADERS += \
    collisioncombobox.h \
    dlg_gamefeild.h \
    gameobject.h \
    gameobjecttype.h \
    mainwindow.h \
    wdg_gameobjectsettingsrow.h

FORMS += \
    dlg_gamefeild.ui \
    mainwindow.ui \
    wdg_gameobjectsettingsrow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
