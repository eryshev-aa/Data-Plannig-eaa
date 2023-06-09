#CONFIG -= qt
TEMPLATE = lib
DEFINES += MYLIB_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    inputfileshandler.cpp \
    mylib.cpp \
    outputresult.cpp \
    tableprolet.cpp

HEADERS += \
    inputfileshandler.h \
    mylib_global.h \
    mylib.h \
    outputresult.h \
    tableprolet.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target
