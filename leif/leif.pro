QT += quick widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        carbonplugindata.cpp \
        main.cpp \
        models/trayiconmodel.cpp \
        trayicon.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    lang/leif_en_GB.ts \
    lang/leif_de_DE.ts

CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += include/

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    carbonplugindata.h \
    include/carbondata.h \
    include/interfaces/IDataProvider.h \
    main.h \
    models/trayiconmodel.h \
    trayicon.h
