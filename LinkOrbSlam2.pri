INCLUDEPATH += $$PWD/thirdparty/include
DEPENDPATH += $$PWD/thirdparty/include
##
win32: LIBS += -L$$PWD/thirdparty/lib/ -lORB_SLAM2

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/lib/ORB_SLAM2.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/lib/libORB_SLAM2.a
##
win32: LIBS += -L$$PWD/thirdparty/lib/ -lDBoW2

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/lib/DBoW2.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/lib/libDBoW2.a
##
win32: LIBS += -L$$PWD/thirdparty/lib/ -lg2o

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/lib/g2o.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/thirdparty/lib/libg2o.a

win32: LIBS += -L$$PWD/thirdparty/lib/ -lopencv_world331
