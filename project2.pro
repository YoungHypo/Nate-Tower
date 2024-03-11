QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Common.cpp \
    boom.cpp \
    bullet.cpp \
    cannon.cpp \
    choosemapscene.cpp \
    custommap.cpp \
    enemy.cpp \
    flamebullet.cpp \
    gamescene.cpp \
    gamesceneinit.cpp \
    gamescenemouse.cpp \
    gamescenepaint.cpp \
    goldpagoda.cpp \
    iceboom.cpp \
    icepagoda.cpp \
    inheritcannon.cpp \
    inheritenemy.cpp \
    insertmap.cpp \
    main.cpp \
    mainwindow.cpp \
    mybutton.cpp \
    pagoda.cpp \
    routepoint.cpp \
    tower.cpp \
    toxicbullet.cpp

HEADERS += \
    Common.h \
    boom.h \
    bullet.h \
    cannon.h \
    choosemapscene.h \
    custommap.h \
    enemy.h \
    gamescene.h \
    insertmap.h \
    mainwindow.h \
    mybutton.h \
    pagoda.h \
    routepoint.h \
    tower.h

FORMS += \
    custommap.ui \
    gamescene.ui \
    insertmap.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

DISTFILES += \
    image/groundpath1.png \
    image/iceFog00.png \
    image/iceFog01.png \
    image/iceFog02.png \
    image/iceFog03.png \
    image/iceFog04.png \
    image/iceFog05.png \
    image/iceFog06.png \
    image/iceFog07.png \
    image/iceFog08.png \
    image/iceFog09.png \
    image/iceFog10.png \
    image/iceFog11.png \
    image/iceFroze00.png \
    image/iceFroze01.png \
    image/iceFroze02.png \
    image/iceFroze03.png

win32: {
        INCLUDEPATH +=  "D:/User/Coding/Visual Leak Detector/include"
        LIBS += -L"D:/User/Coding/Visual Leak Detector/lib/Win32"
}


