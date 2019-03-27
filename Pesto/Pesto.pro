TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp \
    socket.cpp \
    utils.cpp \
    log.cpp \
    thread.cpp \
    fw.cpp \
    memVideo.cpp \
    zscale.c

#pour Lyra
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/NuvuCameras/lib/release/ -lnuvu
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/NuvuCameras/lib/debug/ -lnuvu
#else:unix: LIBS += -L$$PWD/../../../../../opt/NuvuCameras/lib/ -lnuvu

INCLUDEPATH += \
        /usr/local/include/opencv4 \
        /usr/local/include/opencv4/opencv2 \
        /usr/local/include/
LIBS += -L/usr/local/lib/ -L/opt/opencv/release/ -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_stitching -lopencv_imgcodecs


#pour macbook-pro
win32:CONFIG(release, debug|release): LIBS += -L/opt/NuvuCameras/lib/release/ -lnuvu
else:win32:CONFIG(debug, debug|release): LIBS += -L/opt/NuvuCameras/lib/debug/ -lnuvu
else:unix: LIBS += -L/opt/NuvuCameras/lib/ -lnuvu

QMAKE_CXXFLAGS += -std=c++0x -pthread
LIBS += -pthread

#pour lyra
#INCLUDEPATH += $$PWD/../../../../../opt/NuvuCameras/include
#DEPENDPATH += $$PWD/../../../../../opt/NuvuCameras/include

#pour macbook-pro
INCLUDEPATH += /opt/NuvuCameras/include
DEPENDPATH += /opt/NuvuCameras/include

HEADERS += \
    socket.h \
    port.h \
    utils.h \
    var.h \
    log.h \
    structure.h \
    thread.h \
    fw.h \
    memVideo.h \
    zscale.h

#INCLUDEPATH += $$PWD/../../../../../../usr/local/include
#DEPENDPATH += $$PWD/../../../../../../usr/local/include

#INCLUDEPATH += $$PWD/../../../../../../usr/include
#DEPENDPATH += $$PWD/../../../../../../usr/include
