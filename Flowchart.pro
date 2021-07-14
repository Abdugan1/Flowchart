QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/diagramitem.cpp \
    src/diagramscene.cpp \
    src/diagramtextitem.cpp \
    src/diagramview.cpp \
    src/graphicsitemgroup.cpp \
    src/handleitem.cpp \
    src/handleitemappeararea.cpp \
    src/internal.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/pathresizer.cpp \
    src/sizegripitem.cpp

HEADERS += \
    src/diagramitem.h \
    src/diagramscene.h \
    src/diagramtextitem.h \
    src/diagramview.h \
    src/graphicsitemgroup.h \
    src/handleitem.h \
    src/handleitemappeararea.h \
    src/internal.h \
    src/mainwindow.h \
    src/pathresizer.h \
    src/scenebuffer.h \
    src/sizegripitem.h

FORMS +=

RC_ICONS = rsc/images/app_icon.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rsc/resources.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
