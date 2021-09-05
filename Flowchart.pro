QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/arrowhandleitem.cpp \
    src/arrowitem.cpp \
    src/arrowmanager.cpp \
    src/diagramitem.cpp \
    src/diagramscene.cpp \
    src/diagramtextitem.cpp \
    src/diagramview.cpp \
    src/graphicsitemgroup.cpp \
    src/handleitemappeararea.cpp \
    src/handlemanager.cpp \
    src/infolabel.cpp \
    src/internal.cpp \
    src/itemproperties.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/positionline.cpp \
    src/scenebuffer.cpp \
    src/sizegrip.cpp \
    src/sizehandleitem.cpp

HEADERS += \
    src/arrowhandleitem.h \
    src/arrowitem.h \
    src/arrowmanager.h \
    src/constants.h \
    src/diagramitem.h \
    src/diagramscene.h \
    src/diagramtextitem.h \
    src/diagramview.h \
    src/graphicsitemgroup.h \
    src/handleitemappeararea.h \
    src/handlemanager.h \
    src/infolabel.h \
    src/internal.h \
    src/itemproperties.h \
    src/itemtypes.h \
    src/mainwindow.h \
    src/positionline.h \
    src/scenebuffer.h \
    src/sizegrip.h \
    src/sizehandleitem.h

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
