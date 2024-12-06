QT       += core gui multimedia multimediawidgets #添加 multimedia 和 multimediawidgets 模块

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    fire.cpp \
    ght.cpp \
    ghtmp4.cpp \
    ghttimer.cpp \
    main.cpp \
    mainwindow.cpp \
    photoshop.cpp

HEADERS += \
    fire.h \
    ght.h \
    ghtmp4.h \
    ghttimer.h \
    mainwindow.h \
    photoshop.h

FORMS += \
    fire.ui \
    ghttimer.ui \
    mainwindow.ui \
    photoshop.ui



TRANSLATIONS += 229480308-ght-work6_en.ts \
                229480308-ght-work6_zh_CN.ts


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# 输出目录配置
CONFIG(debug, debug|release) {
    DESTDIR = $$PWD/debug
} else {
    DESTDIR = $$PWD/release
}



RESOURCES += \
    image.qrc \
    photo.qrc \
    vedio.qrc \
    voice.qrc

RC_ICONS = hq.ico

DISTFILES +=

# 添加一些编译优化选项
QMAKE_CXXFLAGS_RELEASE += -O2
QMAKE_CXXFLAGS += -Wall -Wextra
