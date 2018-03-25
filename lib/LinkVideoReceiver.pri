INCLUDEPATH += $$PWD/../thirdparty/include
DEPENDPATH += $$PWD/../thirdparty/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../thirdparty/lib/ -lVideoReceiver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../thirdparty/lib/ -lVideoReceiverd

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../thirdparty/lib/ -lMatSerialization
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../thirdparty/lib/ -lMatSerializationd
