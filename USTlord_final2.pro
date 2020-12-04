QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    clientwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    playwindow.cpp \
    serverwindow.cpp \
    source/AIPlayer.cpp \
    source/Board.cpp \
    source/Card.cpp \
    source/CardsGroup.cpp \
    source/CardsType.cpp \
    source/CurrentPattern.cpp \
    source/Deck.cpp \
    source/Player.cpp \
    source/datapackage.cpp\
    source/CardPicture.cpp

HEADERS += \
    clientwindow.h \
    mainwindow.h \
    playwindow.h \
    serverwindow.h \
    source/AIPlayer.h \
    source/Board.h \
    source/Card.h \
    source/CardsGroup.h \
    source/CardsType.h \
    source/CurrentPattern.h \
    source/Deck.h \
    source/Player.h \
    source/datapackage.h\
    source/CardPicture.h

FORMS += \
    clientwindow.ui \
    mainwindow.ui \
    playwindow.ui \
    serverwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc \
    music.qrc
