TEMPLATE = app

QT += 3dcore 3drender 3dinput 3dextras widgets

CONFIG   += console

SOURCES += \
    main.cpp \
    computematerial.cpp \
    computeframegraph.cpp


RESOURCES += \
    compute-particles-cpp.qrc

HEADERS += \
    computematerial.h \
    computeframegraph.h


