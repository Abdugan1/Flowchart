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
    src/internal.h \
    src/mainwindow.h \
    src/pathresizer.h \
    src/sizegripitem.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
