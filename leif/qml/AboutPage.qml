import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

BasePage {
    stateShownIn: "ABOUT"

    Column {
        anchors.centerIn: parent
        spacing: 20

        Image {
            source: "../img/title.png"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        CenteredText {
            text: qsTr("Dekstop App for\nWindows version 1.2")
            lineHeight: 1.5
            horizontalAlignment: Text.AlignHCenter
        }

        CenteredText {
            text: "<a href='https://leif.support/'>https://leif.support</a>"
            onLinkActivated: (link) => Qt.openUrlExternally(link);
        }

        CenteredText {
            text: "Copyright © Tim Stone 2022"
        }
    }
}
