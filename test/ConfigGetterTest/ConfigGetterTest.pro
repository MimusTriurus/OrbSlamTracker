#-------------------------------------------------
#
# Project created by QtCreator 2018-04-02T21:36:21
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ConfigGetterTest
TEMPLATE = app

DESTDIR = ../../bin

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp

HEADERS += \
        MainWindow.h

FORMS += \
        MainWindow.ui

include(../../LinkOpenCv.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/release/ -lCvMatProvider
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/debug/ -lCvMatProvider
else:unix:!macx: LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/ -lCvMatProvider

INCLUDEPATH += $$PWD/../../lib/static/CvMatProvider
DEPENDPATH += $$PWD/../../lib/static/CvMatProvider

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/libCvMatProvider.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/libCvMatProvider.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/CvMatProvider.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/CvMatProvider.lib
else:unix:!macx: PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/libCvMatProvider.a
### mat provider
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/release/ -lCvMatProvider
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/debug/ -lCvMatProvider

INCLUDEPATH += $$PWD/../../lib/static/CvMatProvider
DEPENDPATH += $$PWD/../../lib/static/CvMatProvider

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/libCvMatProvider.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/libCvMatProvider.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/CvMatProvider.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/CvMatProvider.lib
