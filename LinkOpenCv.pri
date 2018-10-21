INCLUDEPATH += $$PWD/thirdparty/include
DEPENDPATH += $$PWD/thirdparty/include

win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_core341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_aruco341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_calib3d341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_features2d341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_flann341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_highgui341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_imgcodecs341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_imgproc341
win32: LIBS += -L$$PWD/thirdparty/lib/opencv/ -lopencv_videoio341
