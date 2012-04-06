#-------------------------------------------------
#
# Project created by QtCreator 2012-01-30T16:38:58
#
#-------------------------------------------------

QT       += network

QT       -= gui

TARGET = QHtsp
TEMPLATE = lib

DEFINES += QHTSP_LIBRARY

SOURCES += qhtspconnection.cpp \
    qhtspmessage.cpp \
    qhtsp.cpp \
    qhtspchannel.cpp \
    qhtsptag.cpp \
    qhtspchannellist.cpp \
    qhtsptaglist.cpp \
    qhtspchannelmodel.cpp \
    qhtsptagmodel.cpp \
    qhtspevent.cpp \
    qhtspeventlist.cpp \
    qhtspeventmodel.cpp \
    qhtspepgquery.cpp \
    qhtspdvrentry.cpp \
    qhtspdvrentrylist.cpp \
    qhtspdvrentrymodel.cpp \
    qhtspchanneldata.cpp \
    qhtspeventdata.cpp \
    qhtspepgquerydata.cpp \
    qhtsptagdata.cpp

HEADERS += qhtspconnection.h \
    qhtsp_global.h \
    qhtspmessage.h \
    qhtsp.h \
    qhtspchannel.h \
    qhtsptag.h \
    qhtspchannellist.h \
    qhtsptaglist.h \
    qhtspchannelmodel.h \
    qhtsptagmodel.h \
    qhtspevent.h \
    qhtspeventlist.h \
    qhtspeventmodel.h \
    qhtspepgquery.h \
    qhtspdvrentry.h \
    qhtspdvrentrylist.h \
    qhtspdvrentrymodel.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1302CF3
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = QHtsp.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
