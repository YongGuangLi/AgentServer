QT += core concurrent
QT -= gui

CONFIG += c++11

TARGET = AgentServer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    filesystemwatcher.cpp \
    processwatcher.cpp \
    netportwatcher.cpp \
    RedisHelper.cpp \
    ServerMonitor.pb.cc \
    confighelper.cpp \
    message_handler.cpp

HEADERS += \
    filesystemwatcher.h \
    processwatcher.h \
    netportwatcher.h \
    RedisHelper.h \
    ServerMonitor.pb.h \
    confighelper.h \
    message_handler.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libacl/lib/release/ -lacl_cpp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libacl/lib/debug/ -lacl_cpp
else:unix: LIBS += -L$$PWD/libacl/lib/ -lacl_cpp

INCLUDEPATH += $$PWD/libacl/include
DEPENDPATH += $$PWD/libacl/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/release/libacl_cpp.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/debug/libacl_cpp.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/release/acl_cpp.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/debug/acl_cpp.lib
else:unix: PRE_TARGETDEPS += $$PWD/libacl/lib/libacl_cpp.a

l
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libacl/lib/release/ -lacl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libacl/lib/debug/ -lacl
else:unix: LIBS += -L$$PWD/libacl/lib/ -lacl

INCLUDEPATH += $$PWD/libacl/include
DEPENDPATH += $$PWD/libacl/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/release/libacl.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/debug/libacl.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/release/acl.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libacl/lib/debug/acl.lib
else:unix: PRE_TARGETDEPS += $$PWD/libacl/lib/libacl.a


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/protobuf-3.3.0/lib/release/ -lprotobuf-lite
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/protobuf-3.3.0/lib/debug/ -lprotobuf-lite
else:unix: LIBS += -L$$PWD/protobuf-3.3.0/lib/ -lprotobuf-lite

INCLUDEPATH += $$PWD/protobuf-3.3.0/include
DEPENDPATH += $$PWD/protobuf-3.3.0/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/protobuf-3.3.0/lib/release/libprotobuf-lite.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/protobuf-3.3.0/lib/debug/libprotobuf-lite.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/protobuf-3.3.0/lib/release/protobuf-lite.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/protobuf-3.3.0/lib/debug/protobuf-lite.lib
else:unix: PRE_TARGETDEPS += $$PWD/protobuf-3.3.0/lib/libprotobuf-lite.a
