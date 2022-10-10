import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

LeifComboBox {
    id: control

    defaultIcon: "../img/pin.png"
    defaultText: qsTr("Select your region")
    model: ["North Scotland", "South Scotland", "North West England", "North East England", "Yorkshire", "North Wales", "South Wales", "West Midlands", "East Midlands", "East England", "South West England", "South England", "London", "South East England", "England", "Scotland", "Wales"]

    delegate: ItemDelegate {
        width: control.width
        palette.text: control.palette.buttonText
        font: control.font
        text: modelData
        highlighted: control.highlightedIndex === index
        spacing: control.spacing
        indicator: Image {
            visible: control.currentIndex === index
            source: control.highlightedIndex === index ? "../img/check_black.png" : "../img/check_white.png"
            x: control.width - width - control.rightInset - control.spacing
            y: control.topPadding + (control.availableHeight - height) / 2
        }
    }

    contentItem: Row {
        spacing: control.spacing
        leftPadding: 20
        rightPadding: control.indicator.width + control.spacing

        Image {
            source: control.defaultIcon
            width: 24
            height: 24
            anchors.verticalCenter: parent.verticalCenter
        }

        Text {
            text: control.currentIndex >= 0 ? control.displayText : control.defaultText
            font: control.font
            color: control.palette.buttonText
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}
