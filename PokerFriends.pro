#-------------------------------------------------
#
# Project created by QtCreator 2019-07-31T16:53:20
#
#-------------------------------------------------

QT       += core network gui quick quickwidgets 3dquickextras
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
        3d/discolightentity.cpp \
        3d/entryanimatedentity.cpp \
        3d/fpscounterentity.cpp \
        3d/playerentity.cpp \
        3d/scene3d.cpp \
        3d/singlecardentity.cpp \
        core/gameplay.cpp \
        core/playersressource.cpp \
        main.cpp \
        networking/client.cpp \
        networking/connection.cpp \
        networking/networkprotocol.cpp \
        networking/peermanager.cpp \
        networking/server.cpp \
        ui/gamewidget.cpp \
        ui/initialmenu.cpp \
        ui/partywidget.cpp \
        ui/size.cpp \
        ui/widget.cpp

HEADERS += \
    3d/discolightentity.h \
    3d/entryanimatedentity.h \
    3d/fpscounterentity.h \
    3d/playerentity.h \
    3d/scene3d.h \
    3d/singlecardentity.h \
    core/gameplay.h \
    core/player.h \
    core/playersressource.h \
    networking/client.h \
    networking/connection.h \
    networking/networkprotocol.h \
    networking/peermanager.h \
    networking/server.h \
    ui/gamewidget.h \
    ui/initialmenu.h \
    ui/partywidget.h \
    ui/size.h \
    ui/widget.h

FORMS += \
    ui/widget.ui

OTHER_FILES += \
    ui/BetPanel.qml \
    ui/BetVerificationPanel.qml \
    ui/FoldVerificationPanel.qml \
    ui/PlayPanel.qml \
    ui/StartTheGamePanel.qml \
    ui/main.qml

CONFIG += mobility
MOBILITY = 


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

OTHER_FILES += \
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

RESOURCES += \
    3dassets.qrc \
    qml.qrc
