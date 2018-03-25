#-------------------------------------------------
#
# Project created by QtCreator 2015-01-11T15:14:10
#
#-------------------------------------------------

QT       += core gui network

TARGET = 3b_CameraControl
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = ../../bin

SOURCES += \
    main.cpp \
    window.cpp \
    camera3d.cpp \
    input.cpp \
    transform3d.cpp

HEADERS += \
    window.h \
    camera3d.h \
    input.h \
    transform3d.h \
    vertex.h

RESOURCES += \
    resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/dynamic/OrbSlamTracker/release/ -lOrbSlamTracker
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/dynamic/OrbSlamTracker/debug/ -lOrbSlamTracker
else:unix:!macx: LIBS += -L$$OUT_PWD/../../lib/dynamic/OrbSlamTracker/ -lOrbSlamTracker

INCLUDEPATH += $$PWD/../../lib/dynamic/OrbSlamTracker
DEPENDPATH += $$PWD/../../lib/dynamic/OrbSlamTracker

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../thirdparty/lib/ -lopencv_world331
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../thirdparty/lib/ -lopencv_world331d

INCLUDEPATH += $$PWD/../../thirdparty/include
DEPENDPATH += $$PWD/../../thirdparty/include

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../thirdparty/lib/ -lVideoReceiver
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../thirdparty/lib/ -lVideoReceiverd

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../thirdparty/lib/ -lMatSerialization
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../thirdparty/lib/ -lMatSerializationd
################################################################################################################
