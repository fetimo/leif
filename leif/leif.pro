QT += quick widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        leifsettings.cpp \
        models/countrymodel.cpp \
        models/settingsmodel.cpp \
        plugin/carbonplugin.cpp \
        main.cpp \
        models/carbonmodel.cpp \
        models/trayiconmodel.cpp \
        plugin/carbonpluginmanager.cpp \
        powerfactory_win.cpp \
        powerinfo.cpp \
        trayicon.cpp \
        utils/carbonplugindata.cpp \
        utils/territory.cpp \
        utils/translatedstring.cpp \
        utils/translation.cpp

RESOURCES += qml.qrc

TRANSLATIONS += \
    lang/leif_en_GB.ts \
    lang/leif_de_DE.ts

CONFIG += lrelease
CONFIG += embed_translations qmltypes

INCLUDEPATH += include/ models/

QML_IMPORT_NAME = Leif
QML_IMPORT_MAJOR_VERSION = 1

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    leifsettings.h \
    models/countrymodel.h \
    models/settingsmodel.h \
    plugin/carbonplugin.h \
    include/carbondata.h \
    include/interfaces/IDataProvider.h \
    include/interfaces/IPower.h \
    main.h \
    models/carbonmodel.h \
    models/trayiconmodel.h \
    plugin/carbonpluginmanager.h \
    powerfactory.h \
    powerinfo.h \
    trayicon.h \
    utils/carbonplugindata.h \
    utils/territory.h \
    utils/translatedstring.h \
    utils/translation.h


win32: LIBS *= PowrProf.lib
