TEMPLATE = app

QT += 3dcore 3drender 3dinput 3dextras widgets

CONFIG   += console

SOURCES += \
    src/main.cpp \
    src/computematerial.cpp \
    src/computeframegraph.cpp


RESOURCES += \
    compute-particles-cpp.qrc

HEADERS += \
    src/computematerial.h \
    src/computeframegraph.h


