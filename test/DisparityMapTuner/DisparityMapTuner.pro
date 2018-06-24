#-------------------------------------------------
#
# Project created by QtCreator 2018-06-07T21:42:11
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DisparityMapTuner
TEMPLATE = app

CONFIG += c++11

DESTDIR = ../../bin

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
    WindowStereoBM.cpp \
    WindowSelectAlgorithm.cpp \
    WindowStereoSGBM.cpp

HEADERS += \
    WindowStereoBM.h \
    WindowSelectAlgorithm.h \
    WindowStereoSGBM.h

INCLUDEPATH += $$PWD/../../thirdparty/include
DEPENDPATH += $$PWD/../../thirdparty/include

include(../../LinkOpenCv.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/release/ -lCvMatProvider
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../lib/static/CvMatProvider/debug/ -lCvMatProvider

INCLUDEPATH += $$PWD/../../lib/static/CvMatProvider
DEPENDPATH += $$PWD/../../lib/static/CvMatProvider

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/libCvMatProvider.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/libCvMatProvider.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/release/CvMatProvider.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../lib/static/CvMatProvider/debug/CvMatProvider.lib
