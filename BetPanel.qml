import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12

import "./" as Path

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

    function hideFirstPanel(){
        mainLayout.visible     = false
    }
    function showFirstPanel(){
        mainLayout.visible     = true
    }

    signal canceled()

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: 0
        Layout.alignment: Qt.AlignHCenter

        OpacityAnimator {
            id: betLayOpacityAnim
            target: betLay
            from: 0
            to: 1
            duration: 1000
            easing.type: Easing.InCirc
        }

        Label {
            id: betPanelLabel
            objectName: "betPanelLabel"
            text: "Bet Panel"
            width: 60*SIZE_FACTOR
            height: 48*SIZE_FACTOR
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
            id: betLay
            Layout.margins: 0
            Layout.fillWidth: true
            Layout.topMargin: 1*SIZE_FACTOR
            visible: true



            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 50*SIZE_FACTOR
            }

            Button {
                id: betButton
                objectName: "betButton"
                text: "Bet"
                implicitWidth: 60*SIZE_FACTOR
                implicitHeight: 48*SIZE_FACTOR
                Layout.margins: 0
                Layout.alignment: Qt.AlignLeft
                highlighted: true
                Material.accent: Material.Teal
                property bool hover: false

                contentItem: Text {
                    text: betButton.text
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
                    betVerificationPanel.visible = true
                    betVerificationPanelEntryAnim.start()
                }
            }
            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 10*SIZE_FACTOR
            }
            SpinBox {
                id: betSpinBox
                objectName: "betSpinBox"
                editable: true
                value: 0
                from: 0
                to: 16777215
                implicitWidth: 150*SIZE_FACTOR
                implicitHeight: 48*SIZE_FACTOR
                font.pixelSize: 20*SIZE_FACTOR
                Layout.margins: 0
                Layout.alignment: Qt.AlignHCenter
                visible: true
            }
            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 80*SIZE_FACTOR
            }
        }

        Button {
            id: cancelButton
            objectName: "cancelButton"
            text: "Cancel"
            Layout.rightMargin: 100*SIZE_FACTOR
            Layout.margins: 0
            Layout.alignment: Qt.AlignRight
            implicitWidth: 60*SIZE_FACTOR
            implicitHeight: 48*SIZE_FACTOR
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
                betVerificationPanel.visible = false
                window.canceled()
            }
        }
    }

    Path.BetVerificationPanel {
        id: betVerificationPanel
        width: APP_SIZEW
        height: APP_SIZEH*SCREEN_PERCENT/100
        focus: true
        color: "#272833"
        visible: true
        y: APP_SIZEH
        onCanceled: {
            betVerificationPanelExitAnim.start()
        }
    }

    PropertyAnimation {
        id: betVerificationPanelEntryAnim
        target: betVerificationPanel
        property: "y"
        from: betVerificationPanel.y
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
        onFinished: {
            window.hideFirstPanel()
        }
    }

    PropertyAnimation {
        id: betVerificationPanelExitAnim
        target: betVerificationPanel
        property: "y"
        from: betVerificationPanel.y
        to: betVerificationPanel.height
        easing.type: Easing.InOutElastic
        duration: 2000
        onFinished: {
            window.showFirstPanel()
            betLay.opacity = 0
            betLayOpacityAnim.start()
        }
    }
}
