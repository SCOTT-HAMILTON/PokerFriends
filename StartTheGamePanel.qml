import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import "./" as Path

Rectangle {
    id: window
    width: APP_SIZEW
    color: "#272822"
    visible: true
    y: 100

    Material.theme: Material.Dark
    Material.accent: Material.Pink

    Layout.alignment: Qt.AlignHCenter

    signal playerBet(var amount)
    signal playerFold()

    function setAllPlayersReady(){
        while (startTheGameButtonPaddingAnimation.running);
        console.log("ready!!!")
        startTheGameButtonPaddingAnimation.stop()
        clippingRect.enabled = true
        startTheGameButtonText.text = "Start the game"
        startTheGameButton.buttonMode = "STARTED"
    }

    function popPlayPanel(){
        windowSlidingAnim.start()
    }

    function startTheGame(){
        if (startTheGameButton.buttonMode !== "STARTED_BY_US"){
            windowSlidingDownAnim.start()
            clippingRect.visible = false
        }
    }

    signal gameStarted()
    signal readyWaiting()

    RowLayout {
        id: buttonsLay
        Layout.margins: 0
        Layout.fillWidth: true
        Layout.topMargin: 1*SIZE_FACTOR
        visible: true

        Item {
            // spacer item
            Layout.fillHeight: true
            implicitWidth: 30*SIZE_FACTOR
        }
        Rectangle {
            id: clippingRect
            implicitWidth: APP_SIZEW*0.8
            implicitHeight: 40*SIZE_FACTOR
            clip: true

            Layout.margins: 0
            Layout.alignment: Qt.AlignLeft
            Layout.topMargin: 30*SIZE_FACTOR

            color: window.color

            Button {
                id: startTheGameButton
                anchors.fill: parent
                anchors.topMargin: -6*SIZE_FACTOR
                anchors.bottomMargin: -6*SIZE_FACTOR
                objectName: "startTheGameButton"
                text: "Ready to start the game"
                property string buttonMode: "READY"
                highlighted: true
                Material.accent: Material.Teal
                property bool hover: false

                contentItem: Text {
                    id: startTheGameButtonText
                    parent: startTheGameButton
                    text: startTheGameButton.text
                    font.pixelSize: 16*SIZE_FACTOR
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

                OpacityAnimator {
                    id: startTheGameButtonOpacityAnim
                    target: startTheGameButton
                    from: 1
                    to: 0
                    duration: 1000
                    easing.type: Easing.OutQuad
                    onFinished: {
                        windowSlidingAnim.start()
                    }
                }

                PropertyAnimation {
                    id: startTheGameButtonPaddingAnimation
                    target: startTheGameButton
                    property: "topPadding"
                    from: startTheGameButton.topPadding
                    to: startTheGameButton.height*2
                    easing.type: Easing.Linear
                    property int myduration: 500
                    duration: myduration.valueOf()
                    onFinished:{
                        if (startTheGameButton.buttonMode == "READY_WAITING"){
                            clippingRect.enabled = false
                            readyWaiting()
                        }
                        if (startTheGameButton.buttonMode == "START"){
                            console.log("yep start")
                            startTheGameButton.buttonMode = "STARTED"
                            startTheGameButtonText.text = "Start the game"
                            startTheGameButtonPaddingAnimation.from = -startTheGameButton.height*0.7
                            startTheGameButtonPaddingAnimation.to = 12*SIZE_FACTOR
                            startTheGameButtonPaddingAnimation.duration =
                            startTheGameButtonPaddingAnimation.myduration/2
                            startTheGameButtonPaddingAnimation.start()
                        }
                        else if (startTheGameButton.buttonMode == "READY"){
                            startTheGameButton.buttonMode = "READY_WAITING"
                            startTheGameButtonText.text = "Waiting for other players to be ready..."
                            startTheGameButtonPaddingAnimation.from = -startTheGameButton.height*0.7
                            startTheGameButtonPaddingAnimation.to = 12*SIZE_FACTOR
                            startTheGameButtonPaddingAnimation.duration =
                            startTheGameButtonPaddingAnimation.myduration/2
                            startTheGameButtonPaddingAnimation.start()
                        }else{
                            startTheGameButtonPaddingAnimation.from = 12*SIZE_FACTOR
                            startTheGameButtonPaddingAnimation.to = startTheGameButton.height*2
                            startTheGameButtonPaddingAnimation.duration =
                            startTheGameButtonPaddingAnimation.myduration
                        }
                    }
                }

                onClicked: {
                    if (startTheGameButton.buttonMode == "STARTED"){
                        startTheGameButton.buttonMode = "STARTED_BY_US"
                        startTheGameButtonOpacityAnim.start()
                        gameStarted()
                    }
                    else startTheGameButtonPaddingAnimation.start()

                }
            }
        }

        Item {
            // spacer item
            Layout.fillHeight: true
            implicitWidth: 30*SIZE_FACTOR
        }
    }

    Path.PlayPanel{
        id: playPanel
        width: APP_SIZEW
        height: APP_SIZEH*SCREEN_PERCENT/100
        focus: true
        visible: true
        x: APP_SIZEW
        onBet: {
            playerBet(amount)
            windowSlidingDownAnim.start()
        }

        onFold: {
            playerFold()
            windowSlidingDownAnim.start()
        }
    }

    PropertyAnimation {
        id: playPanelEntryAnim
        target: playPanel
        property: "x"
        from: playPanel.x
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
    }

    PropertyAnimation {
        id: windowSlidingAnim
        target: window
        property: "y"
        from: window.y
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
        onFinished: {
            if (startTheGameButton.buttonMode == "STARTED_BY_US"){

                playPanelEntryAnim.start()
            }

        }
    }

    PropertyAnimation {
        id: windowSlidingDownAnim
        target: window
        property: "y"
        from: window.y
        to: window.height
        easing.type: Easing.OutCubic
        duration: 1000
        onFinished: {
            if (startTheGameButton.buttonMode == "STARTED_BY_US")
                playPanelEntryAnim.start()

        }
    }
}
