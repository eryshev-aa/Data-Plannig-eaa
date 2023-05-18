QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        finddatafilenames.cpp \
        main.cpp \
        #restrictionsparser.cpp \
        #zrvparser.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix:!macx: LIBS += -L$$PWD/../build-mylib-Desktop-Debug/ -lmylib

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Release/release/ -lmylib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug/ -lmylib

INCLUDEPATH += $$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug
DEPENDPATH += $$PWD/../build-mylib-Desktop_Qt_5_15_2_MinGW_64_bit-Debug/debug

INCLUDEPATH += $$PWD/../mylib

INCLUDEPATH += $$PWD/../build-mylib-Desktop-Debug
DEPENDPATH += $$PWD/../build-mylib-Desktop-Debug

DISTFILES +=

RESOURCES +=

HEADERS += \
    finddatafilenames.h \
    #restrictionsparser.h \
    #zrvparser.h




