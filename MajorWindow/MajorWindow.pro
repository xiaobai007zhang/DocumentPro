QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
INCLUDEPATH += ../Journal
INCLUDEPATH += ../ViewCheck
#INCLUDEPATH += ../ysbase/include/
#LIBS += -LD:\QtProject\DocumentPro\ysbase\ -lysbase

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


FORMS += \
    Major/cmajor.ui \
    ManagerTableInfo/managertableinfo.ui

HEADERS += \
    AbstractShape/abstractshape.h \
    Major/cmajor.h \
    ManagerTableInfo/managertableinfo.h \
    MyTable/mytable.h \
    PixmapGraphics/mygraphicspixmapitem.h \
    SceneGraphics/mygraphicsscene.h \
    ShapeMimeData/shapemimedata.h \
    TextItemGraphics/mygraphicstextitem.h \
    ViewGraphics/mygraphicsview.h

SOURCES += \
    AbstractShape/abstractshape.cpp \
    App/main.cpp \
    Major/cmajor.cpp \
    ManagerTableInfo/managertableinfo.cpp \
    MyTable/mytable.cpp \
    PixmapGraphics/mygraphicspixmapitem.cpp \
    SceneGraphics/mygraphicsscene.cpp \
    ShapeMimeData/shapemimedata.cpp \
    TextItemGraphics/mygraphicstextitem.cpp \
    ViewGraphics/mygraphicsview.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../ViewCheck/release/ -lViewCheck
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../ViewCheck/debug/ -lViewCheck

INCLUDEPATH += $$PWD/../ViewCheck
DEPENDPATH += $$PWD/../ViewCheck
