QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
LIBS += -lsqlite3

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttonbox.cpp \
    main.cpp \
    buttonwall.cpp \
    chatmessage.cpp \
    friendsmenu.cpp \
    lobbygame.cpp \
    login.cpp \
    mainmenu.cpp \
    profil.cpp \
    mainwindow.cpp \
    board.cpp \
    client.cpp \
    common.cpp \
    database.cpp \
    game.cpp \
    lobby.cpp \
    mainclient.cpp \
    mainserver.cpp \
    masterserver.cpp \
    player.cpp \
    server.cpp \
    timer.cpp \
    utils.cpp \
    wall.cpp

HEADERS += \
    buttonbox.h \
    buttonwall.h \
    chatmessage.h \
    friendsmenu.h \
    lobbygame.h \
    login.h \
    mainmenu.h \
    observer_subject.h \
    profil.h \
    mainwindow.h \
    board.h \
    client.h \
    common.h \
    database.h \
    game.h \
    lobby.h \
    masterserver.h \
    player.h \
    server.h \
    timer.h \
    utils.h \
    wall.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
