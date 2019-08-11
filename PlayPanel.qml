import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import "./" as Path

Rectangle {
    id: window
    width: SCREENA_SIZEW
    height: APP_SIZEH*SCREEN_PERCENT/100
    focus: true
    color: "#272822"
    visible: true
    Layout.margins: 0

    function hideFirstPanel(){
        yourTurnLabel.visible = false
        betButton.visible     = false
        orLabel.visible       = false
        foldButton.visible    = false
    }
    function showFirstPanel(){
        yourTurnLabel.visible = true
        betButton.visible     = true
        orLabel.visible       = true
        foldButton.visible    = true
    }

    signal bet(var amount)
    signal fold()

    Material.theme: Material.Dark
    Material.accent: Material.Pink

    ColumnLayout {
        id: mainLayout
        anchors.fill: parent
        height: APP_SIZEH
        width: SCREENA_SIZEW
        spacing: 0

        Label {
            id: yourTurnLabel
            objectName: "yourTurnLabel"
            text: "It's your turn"
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

            OpacityAnimator {
                id: buttonsLayOpacityAnim
                target: buttonsLay
                from: 0
                to: 1
                duration: 1000
                easing.type: Easing.OutQuad
            }

            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 80*SIZE_FACTOR
            }

            Button {
                id: betButton
                objectName: "betButton"
                text: "Bet"

                implicitWidth: 70*SIZE_FACTOR
                implicitHeight: 56*SIZE_FACTOR
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
                    window.hideFirstPanel();
                    betPanelEntryAnim.start()
                }
            }
            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 10*SIZE_FACTOR
            }
            Label {
                id: orLabel
                objectName: "orLabel"
                text: " Or "
                font.pixelSize: 20*SIZE_FACTOR
                Layout.margins: 0
                horizontalAlignment: Text.AlignLeft
                visible: true
            }
            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 10*SIZE_FACTOR
            }
            Button {
                id: foldButton
                objectName: "foldButton"
                text: "Fold"
                implicitWidth: 70*SIZE_FACTOR
                implicitHeight: 56*SIZE_FACTOR
                Layout.margins: 0
                Layout.alignment: Qt.AlignLeft
                highlighted: true
                Material.accent: Material.Teal
                property bool hover: false

                contentItem: Text {
                    text: foldButton.text
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
                    window.hideFirstPanel();
                    foldVerificationPanelEntryAnim.start()
                }
            }
            Item {
                // spacer item
                Layout.fillHeight: true
                implicitWidth: 80*SIZE_FACTOR
            }
        }
    }

    Path.BetPanel {
        id: betPanel
        width: SCREENA_SIZEW
        height: APP_SIZEH*SCREEN_PERCENT/100
        focus: true
        visible: true
        x: SCREENA_SIZEW
        onCanceled: {
            betPanelExitAnim.start()
        }
        onBetValidated: {
            bet(amount)
        }
    }

    Path.FoldVerificationPanel {
        id: foldVerificationPanel
        width: SCREENA_SIZEW
        height: APP_SIZEH*SCREEN_PERCENT/100
        focus: true
        visible: true
        x: SCREENA_SIZEW
        onCanceled: {
            foldVerificationPanelExitAnim.start()
        }
        onValidated: {
            fold()
        }
    }

    PropertyAnimation {
        id: betPanelEntryAnim
        target: betPanel
        property: "x"
        from: betPanel.x
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
    }

    PropertyAnimation {
        id: betPanelExitAnim
        target: betPanel
        property: "x"
        from: betPanel.x
        property int exitX: -SCREENA_SIZEW
        to: exitX
        easing.type: Easing.InOutElastic
        duration: 2000
        onFinished: {
            buttonsLay.opacity = 0
            window.showFirstPanel()
            buttonsLayOpacityAnim.start()
            exitX = exitX+(-exitX*2)
        }
    }


    PropertyAnimation {
        id: foldVerificationPanelEntryAnim
        target: foldVerificationPanel
        property: "x"
        from: foldVerificationPanel.x
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
    }

    PropertyAnimation {
        id: foldVerificationPanelExitAnim
        target: foldVerificationPanel
        property: "x"
        from: foldVerificationPanel.x
        property int exitX: -SCREENA_SIZEW
        to: exitX
        easing.type: Easing.InOutElastic
        duration: 2000
        onFinished: {
            buttonsLay.opacity = 0
            window.showFirstPanel()
            buttonsLayOpacityAnim.start()
            exitX = exitX+(-exitX*2)
        }
    }
}
