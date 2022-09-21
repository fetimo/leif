import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: rootWindow

    width: 640
    height: 480
    visible: true
    title: qsTr("Leif - Not just green")

    property int sessionCarbon: 0
    property int lifeTimeCarbon: 27

    Behavior on sessionCarbon {
        NumberAnimation {
            duration: 500
            easing.type: Easing.InOutQuad
        }
    }

    Timer {
        interval: 3000
        repeat: true
        running: true
        onTriggered: {
            rootWindow.sessionCarbon += 3;
            rootWindow.lifeTimeCarbon += 3;
        }
    }

    Image {
        anchors.centerIn: parent
        width: 1.2 * parent.width
        height: 1.2 * parent.height
        source: "../img/background.png"

        transform: Translate {
            id: translate
        }

        ParallelAnimation {
            running: true
            loops: Animation.Infinite

            SequentialAnimation
            {
                PropertyAnimation {
                    target: translate
                    property: "x"
                    from: 0
                    to: 20
                    duration: 7000
                    easing.type: Easing.InOutQuad
                }

                PropertyAnimation {
                    target: translate
                    property: "x"
                    from: 20
                    to: 0
                    duration: 7000
                    easing.type: Easing.InOutQuad
                }
            }

            SequentialAnimation {
                PauseAnimation {
                    duration: 5000
                }
                PropertyAnimation {
                    target: translate
                    property: "y"
                    from: 0
                    to: 20
                    duration: 7000
                    easing.type: Easing.InOutQuad
                }


                PropertyAnimation {
                    target: translate
                    property: "y"
                    from: 20
                    to: 0
                    duration: 7000
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

    Row {
        id: buttonRow

        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 0
        height: statusButton.height

        Rectangle {
            id: statusButton

            color: "transparent"
            width: statusLabel.width + 40
            height: statusLabel.height + 20

            Text {
                id: statusLabel

                anchors.centerIn: parent
                color: statusMouseArea.containsMouse ? "#E91E63" : "white"
                font.pointSize: 25
                text: "STATUS"

                Behavior on color {
                    ColorAnimation {
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            MouseArea {
                id: statusMouseArea

                anchors.fill: parent
                hoverEnabled: true
                onClicked: buttonIndicator.pointTo(statusLabel)
            }
        }

        Rectangle {
            width: 2
            height: statusLabel.height
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
        }

        Rectangle {
            color: "transparent"
            width: locationLabel.width + 40
            height: locationLabel.height + 20

            Text {
                id: locationLabel

                anchors.centerIn: parent
                color: locationMouseArea.containsMouse ? "#E91E63" : "white"
                font.pointSize: 25
                text: "LOCATION"

                Behavior on color {
                    ColorAnimation {
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            MouseArea {
                id: locationMouseArea

                anchors.fill: parent
                hoverEnabled: true
                onClicked: buttonIndicator.pointTo(locationLabel)
            }
        }

        Rectangle {
            width: 2
            height: statusLabel.height
            anchors.verticalCenter: parent.verticalCenter
            color: "white"
        }

        Rectangle {
            color: "transparent"
            width: aboutLabel.width + 40
            height: aboutLabel.height + 20

            Text {
                id: aboutLabel

                anchors.centerIn: parent
                color: aboutMouseArea.containsMouse ? "#E91E63" : "white"
                font.pointSize: 25
                text: "ABOUT"

                Behavior on color {
                    ColorAnimation {
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            MouseArea {
                id: aboutMouseArea

                anchors.fill: parent
                hoverEnabled: true
                onClicked: buttonIndicator.pointTo(aboutLabel)
            }
        }
    }

    Rectangle {
        id: buttonIndicator

        function pointTo(text) {
            width = text.width;
            x = buttonRow.x + text.parent.x + 20;
        }

        width: statusLabel.width
        height: 3
        color: "#E91E63"

        x: buttonRow.x + statusLabel.x
        y: buttonRow.y + buttonRow.height

        Behavior on width {
            PropertyAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }

        Behavior on x {
            PropertyAnimation {
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }

    Row {
        anchors.top: parent.top
        anchors.topMargin: 100
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 160

        Column {
            Text {
                color: "white"
                text: "Session"
                font.pointSize: 15
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                color: "white"
                text: rootWindow.sessionCarbon
                font.pointSize: 60
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                color: "#bbbbbb"
                text: "grCO2"
                font.pointSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }


        Column {
            spacing: 5

            Text {
                color: "white"
                text: "Lifetime"
                font.pointSize: 15
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                color: "white"
                text: rootWindow.lifeTimeCarbon
                font.pointSize: 60
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                color: "#bbbbbb"
                text: "grCO2"
                font.pointSize: 12
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
