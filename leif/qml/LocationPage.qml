import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Leif 1.0

BasePage {
    stateShownIn: "LOCATION"

    SettingsModel {
        id: settings
    }

    Item {
        anchors.fill: parent
        anchors.margins: 30

        Column {
            spacing: 30
            anchors.centerIn: parent
            width: parent.width

            CenteredText {
                font.pointSize: 16
                text: qsTr("Leif needs your location to give the most accurate CO₂ readings.")
                wrapMode: Text.Wrap
                width: parent.width
            }

            CountryComboBox {
                id: countryBox
            }

            RegionComboBox {
                id: control
            }
        }
    }
}
