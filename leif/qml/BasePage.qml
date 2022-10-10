import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Item {
    id: root

    property string stateShownIn

    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.top: parent.top
    anchors.topMargin: 100
    anchors.left: parent.horizontalCenter

    visible: false
    opacity: 0

    state: stateHandler.state === stateShownIn ? "shown" : "hidden"
    states: [
        State {name: "hidden"},
        State {
            name: "shown"

            PropertyChanges {
                target: root
                visible: true
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition {
            to: "shown"

            SequentialAnimation {

                PropertyAction {
                    target: root
                    property: "visible"
                }

                PropertyAnimation {
                    target: root
                    property: "opacity"
                    duration: 300
                    easing.type: Easing.InOutQuad
                }
            }
        },

        Transition {
            to: "hidden"

            SequentialAnimation {

                PropertyAnimation {
                    target: root
                    property: "opacity"
                    duration: 300
                    easing.type: Easing.InOutQuad
                }

                PropertyAction {
                    target: root
                    property: "visible"
                }
            }
        }
    ]
}
