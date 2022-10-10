import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

LeifComboBox {
    id: control

    defaultIcon: "../img/globe.png"
    defaultText: qsTr("Select your country")
    textRole: "name"
    valueRole: "iso"
    model: [
        { name: "France", iso: "fr"},
        { name: "Germany", iso: "de" },
        { name: "Republic of Ireland", iso: "irl"},
        { name: "Italy", iso: "ita"},
        { name: "Spain", iso: "esp"},
        {name: "United Kingdom", iso: "gbr"}
    ]

    delegate: ItemDelegate {
        width: control.width
        palette.text: control.palette.buttonText
        font: control.font
        text: modelData.name
        highlighted: control.highlightedIndex === index
        icon.source: "../img/flags/" + modelData.iso + ".png"
        icon.color: "transparent"
        spacing: control.spacing
        indicator: Image {
            visible: control.currentIndex === index
            source: control.highlightedIndex === index ? "../img/check_black.png" : "../img/check_white.png"
            x: control.width - width - control.rightInset - control.spacing
            y: control.topPadding + (control.availableHeight - height) / 2
        }
    }
}
