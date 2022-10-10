import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ComboBox {
    id: control

    property url defaultIcon
    property string defaultText

    editable: false
    palette.window: "#253B45"
    palette.button: "#253B45"
    palette.buttonText: "#f0f2f1"
    font.pointSize: 12
    font.family: "Inter"
    width: parent.width
    spacing: 20
    currentIndex: -1


    background: Rectangle {
        radius: 8
        color: control.palette.button
        opacity: 0.8
        implicitHeight: 48
        implicitWidth: 120

        Behavior on border.color {
            ColorAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }

        border.width: 1
        border.color: control.hovered ? "white" : "#01ffffff"
    }

    contentItem: Row {
        spacing: control.spacing
        leftPadding: 20
        rightPadding: control.indicator.width + control.spacing

        Image {
            source: control.currentIndex >= 0 ? "../img/flags/" + control.currentValue + ".png" : control.defaultIcon
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

    popup: Popup {
        y: control.height - (implicitHeight / 2)
        width: control.width
        implicitHeight: contentItem.implicitHeight
        verticalPadding: 10
        horizontalPadding: 0

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex

            ScrollIndicator.vertical: ScrollIndicator {}
        }

        background: Rectangle {
            color: control.palette.button
            radius: 8
        }
    }

    indicator: Image {
        source: "../img/cbindicator.png"
        x: control.width - width - control.rightInset - control.spacing
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 24
        height: 24
    }
}
