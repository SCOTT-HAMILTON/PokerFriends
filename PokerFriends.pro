#-------------------------------------------------
#
# Project created by QtCreator 2019-07-31T16:53:20
#
#-------------------------------------------------

QT       += core network gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
requires(qtConfig(udpsocket))
requires(qtConfig(listwidget))

TARGET = PokerFriends
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        client.cpp \
        connection.cpp \
        gameplay.cpp \
        gamewidget.cpp \
        initialmenu.cpp \
        main.cpp \
        peermanager.cpp \
        playerwidget.cpp \
        server.cpp \
        widget.cpp

HEADERS += \
        client.h \
        connection.h \
        gameplay.h \
        gamewidget.h \
        initialmenu.h \
        peermanager.h \
        player.h \
        playerwidget.h \
        server.h \
        widget.h

FORMS += \
        gamewidget.ui \
        playerwidget.ui \
        widget.ui

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

contains(ANDROID_TARGET_ARCH,arm64-v8a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
