import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

Rectangle {
    id: window
    width: APP_SIZEW
    height: APP_SIZEH*SCREEN_PERCENT/100
    focus: true
    color: "#272833"
    visible: true

    Material.theme: Material.Dark
    Material.accent: Material.Pink

    Layout.alignment: Qt.AlignHCenter

    signal canceled()
    signal validated()

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        height: APP_SIZEH
        width: APP_SIZEW
        spacing: 0
        Layout.alignment: Qt.AlignHCenter

        Label {
            id: verificationLabel
            objectName: "verificationLabel"
            text: "Are you sure you wanna bet ?"
            width: APP_SIZEW
            font.pixelSize: 20*SIZE_FACTOR
            font.bold: true
            font.family: "Roboto"
            Layout.margins: 0
            Layout.fillWidth: true
            Layout.topMargin: 1*SIZE_FACTOR
            horizontalAlignment: Text.AlignHCenter
            visible: true
        }

        RowLayout {
            id: buttonsLay
            Layout.margins: 0
            Layout.fillWidth: true
            Layout.topMargin: 1*SIZE_FACTOR
            visible: true

            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 90*SIZE_FACTOR
            }

            Button {
                id: cancelButton
                objectName: "cancelButton"
                text: "Cancel"
                implicitWidth: 70*SIZE_FACTOR
                implicitHeight: 56*SIZE_FACTOR
                Layout.margins: 0
                Layout.alignment: Qt.AlignHCenter
                highlighted: true
                Material.accent: Material.Teal
                property bool hover: false

                contentItem: Text {
                    text: cancelButton.text
                    font.pixelSize: 12*SIZE_FACTOR
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onHoveredChanged: {
                    if (hover)
                        Material.accent = Material.Teal
                    else
                        Material.accent = Material.Brown
                    hover = !hover
                }

                onClicked: {
                    window.canceled()
                }
            }
            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 50*SIZE_FACTOR
            }
            Button {
                id: validateButton
                objectName: "validate"
                text: "Validate"
                implicitWidth: 70*SIZE_FACTOR
                implicitHeight: 56*SIZE_FACTOR
                Layout.margins: 0
                Layout.alignment: Qt.AlignHCenter
                highlighted: true
                Material.accent: Material.Teal
                property bool hover: false

                contentItem: Text {
                    text: validateButton.text
                    font.pixelSize: 12*SIZE_FACTOR
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onHoveredChanged: {
                    if (hover)
                        Material.accent = Material.Teal
                    else
                        Material.accent = Material.Brown
                    hover = !hover
                }

                onClicked: {
                    window.validated()
                }
            }
        }
    }
}
