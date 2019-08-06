import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.13

Rectangle {
    id: window
    width: APP_SIZEW
    height: APP_SIZEH
    focus: true
    color: "#272822"
    visible: true


    Material.theme: Material.Dark
    Material.accent: Material.Pink

    signal joinLocalServer()

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        height: APP_SIZEH
        width: APP_SIZEW
        spacing: 0

        Label {
            id: nicknameRefusedLabel
            objectName: "nicknameRefusedLabel"
            text: "This nickname is already used, choose another please."
            width: APP_SIZEW
            color: "#FF0000"
            Layout.margins: 0
            Layout.fillWidth: true
            Layout.topMargin: 20*SIZE_FACTOR
            horizontalAlignment: Text.AlignHCenter
            visible: false
        }
        Item {
            // spacer item
            Layout.fillWidth: true
            implicitHeight: 100*SIZE_FACTOR
        }
        Label {
            id: nicknameLabel
            text: "Your nickname is ?"
            font.pixelSize: 20*SIZE_FACTOR
            Layout.margins: 0
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
        }

        Item {
            // spacer item
            Layout.fillWidth: true
            implicitHeight: 15*SIZE_FACTOR
        }

        TextField {
            id: nicknameInput
            objectName: "nicknameInput"
            text: ""
            font.pixelSize: 20*SIZE_FACTOR
            placeholderText: "Nickname"
            implicitWidth: 200*SIZE_FACTOR
            Layout.margins: 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            horizontalAlignment: Text.AlignHCenter
        }

        Item {
            // spacer item
            Layout.fillWidth: true
            implicitHeight: 50*SIZE_FACTOR
        }

        Button {
            id: joinLocalServerButton
            objectName: "joinLocalServerButton"
            text: "Join the local server"

            Layout.margins: 0
            Layout.alignment: Qt.AlignHCenter
            implicitWidth: 280*SIZE_FACTOR
            implicitHeight: 70*SIZE_FACTOR
            highlighted: true
            Material.accent: Material.Teal
            property bool hover: false

            contentItem: Text {
                text: joinLocalServerButton.text
                font.pixelSize: 20*SIZE_FACTOR
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
                window.joinLocalServer()
            }



        }
        Item {
            // spacer item
            Layout.fillWidth: true
            implicitHeight: 200*SIZE_FACTOR
        }
    }
}
