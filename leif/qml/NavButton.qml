import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Button {
    id: root

    property string shownState
    property bool currentButton: stateHandler.state === shownState
    readonly property color activeColor: "#72e36b"
    property color currentColor: currentButton ? activeColor : "white"

    onClicked: stateHandler.state = shownState;

    enabled: !currentButton
    horizontalPadding: 0

    palette.buttonText: currentColor
    font.pointSize: 14
    font.family: "Inter"

    background: Rectangle {
        color: "transparent"
    }

    indicator: Rectangle {
        anchors.bottom: parent.bottom
        width: root.width
        height: 3
        color: root.activeColor
        opacity: root.currentButton ? 1 : 0

        Behavior on opacity {
            PropertyAnimation {
                duration: 300
                easing.type: Easing.InOutQuad
            }
        }
    }

    Behavior on currentColor {
        ColorAnimation {
            duration: 300
            easing.type: Easing.InOutQuad
        }
    }
}
