
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android-sources

QT += sql qml quick widgets quickcontrols2

android {
    QT += androidextras
}

SOURCES += main.cpp \
    aims/AimBase.cpp \
    aims/AimNotifications.cpp \
    aims/DoneActsChart.cpp \
    aims/ListOperations.cpp \
    aims/RunningAims.cpp \
    aims/ScreenGlobal.cpp \
    aims/UserSettings.cpp \
    aims/UserToken.cpp \
    apptools/LogHandler.cpp \
    apptools/Popup.cpp \
    apptools/SystemTray.cpp \
    apptools/TabelSerialize.cpp \
    apptools/TreeModel.cpp

RESOURCES += qml.qrc \
    icons.qrc



DISTFILES += \
    android-sources/AndroidManifest.xml \
    android-sources/build.gradle \
    android-sources/gradle.properties \
    android-sources/gradle/wrapper/gradle-wrapper.jar \
    android-sources/gradle/wrapper/gradle-wrapper.properties \
    android-sources/gradlew \
    android-sources/gradlew.bat \
    general_notes \
    android-sources/src/at/wavespl/apps/aim/NotificationClient.java \
    TODO \
    android-sources/src/at/wavespl/apps/aim/NotificationService.java


HEADERS += \
    aims/AimBase.h \
    aims/AimNotifications.h \
    aims/DoneActsChart.h \
    aims/ListOperations.h \
    aims/RunningAims.h \
    aims/ScreenGlobal.h \
    aims/UserSettings.h \
    aims/UserToken.h \
    apptools/Clipboard.h \
    apptools/LogHandler.h \
    apptools/Notifications.h \
    apptools/Popup.h \
    apptools/SystemTray.h \
    apptools/TabelSerialize.h \
    apptools/TreeModel.h


INSTALLS += target
