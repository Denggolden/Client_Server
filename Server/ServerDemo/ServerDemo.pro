QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#greaterThan(QT_MAJOR_VERSION,4){
#        TARGET_ARCH=$${QT_ARCH}
#}else{
#        TARGET_ARCH=$${QMAKE_HOST.arch}
#}
#contains(TARGET_ARCH, x86_64){
#    CONFIG(debug,debug|release){
#        MOC_DIR = build/x64/tmp_debug/moc
#        RCC_DIR = build/x64/tmp_debug/rcc
#        UI_DIR = build/x64/tmp_debug/ui
#        OBJECTS_DIR = build/x64/tmp_debug/obj
#        DESTDIR = build/x64/bin/debug/
#    }else{
#        MOC_DIR = build/x64/tmp_release/moc
#        RCC_DIR = build/x64/tmp_release/rcc
#        UI_DIR = build/x64/tmp_release/ui
#        OBJECTS_DIR = build/x64/tmp_release/obj
#        DESTDIR = build/x64/bin/release/
#    }
#}else{
#    CONFIG(debug,debug|release){
#        MOC_DIR = build/x86/tmp_debug/moc
#        RCC_DIR = build/x86/tmp_debug/rcc
#        UI_DIR = build/x86/tmp_debug/ui
#        OBJECTS_DIR = build/x86/tmp_debug/obj
#        DESTDIR = build/x86/bin/debug/
#    }else{
#        MOC_DIR = build/x86/tmp_release/moc
#        RCC_DIR = build/x86/tmp_release/rcc
#        UI_DIR = build/x86/tmp_release/ui
#        OBJECTS_DIR = build/x86/tmp_release/obj
#        DESTDIR = build/x86/bin/release/
#    }
#}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include($$PWD/Src/ComHandleModel/ComHandleModel.pri)
include($$PWD/Src/DataHandleModel/DataHandleModel.pri)
include($$PWD/Src/FileHandleModel/FileHandleModel.pri)
include($$PWD/Src/NetHandleModel/NetHandleModel.pri)
include($$PWD/Src/TestHandleModel/TestHandleModel.pri)
include(../../Common/Common.pri)

FORMS += \
    ServerMainWin.ui

HEADERS += \
    ServerMainWin.h

SOURCES += \
    ServerMainWin.cpp \
    main.cpp
