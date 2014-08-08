# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = IPSteckdose

CONFIG += sailfishapp

SOURCES += src/IPSteckdose.cpp \
    src/ipsmanager.cpp

OTHER_FILES += qml/IPSteckdose.qml \
    qml/cover/CoverPage.qml \
    rpm/IPSteckdose.changes.in \
    rpm/IPSteckdose.spec \
    rpm/IPSteckdose.yaml \
    translations/*.ts \
    IPSteckdose.desktop \
    qml/pages/MainPage.qml \
    qml/pages/SettingsDialog.qml

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n
TRANSLATIONS += translations/IPSteckdose-de.ts

HEADERS += \
    src/ipsmanager.h

