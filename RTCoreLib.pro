#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T09:30:47
#
#-------------------------------------------------

QT       += gui xml network

TARGET = RTCoreLib
TEMPLATE = lib

DEFINES += RTCORELIB_LIBRARY

INCLUDEPATH += C:/local/boost_1_62_0

SOURCES += rtcorelib.cpp \
    size3d.cpp \
    rect3d.cpp \
    lineseg.cpp \
    triangle.cpp \
    point3d.cpp \
    vector3d.cpp \
    quaternion3d.cpp \
    matrix3d.cpp \
    aabbtree.cpp \
    meshbuilder.cpp \
    astar.cpp \
    clustering.cpp \
    fusion.cpp \
    contourhelper.cpp \
    cylindercollision.cpp \
    distancetransform.cpp \
    instanceparameters.cpp \
    printaction.cpp \
    printactioncollection.cpp \
    supportactions.cpp \
    mainviewmodel.cpp \
    mvmparameters.cpp \
    supportgenerator.cpp \
    modelentity.cpp \
    support.cpp

HEADERS += rtcorelib.h\
        rtcorelib_global.h \
    size3d.h \
    rect3d.h \
    lineseg.h \
    triangle.h \
    point3d.h \
    vector3d.h \
    constants.h \
    quaternion3d.h \
    matrix3d.h \
    aabbtree.h \
    meshbuilder.h \
    support.h \
    rcexceptions.h \
    astar.h \
    broadcaststructs.h \
    clustering.h \
    fusion.h \
    contourhelper.h \
    cylindercollision.h \
    distancetransform.h \
    instanceparameters.h \
    boardtype.h \
    printaction.h \
    printactioncollection.h \
    supportactions.h \
    mainviewmodel.h \
    mvmparameters.h \
    supportgenerator.h \
    modelentity.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
