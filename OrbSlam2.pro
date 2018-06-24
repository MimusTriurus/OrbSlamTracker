TEMPLATE = subdirs

QMAKE_CXXFLAGS_RELEASE += -MP10

CONFIG += ordered

SUBDIRS += \
    lib \
    app \
    test

#include(LinkOrbSlam2.pri)
#include(LinkOpenCv.pri)
