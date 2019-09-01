import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

import "./" as Path

Item {
    id: mainWindow
    width: SCREENA_SIZEW
    height: APP_SIZEH*SCREEN_PERCENT/100
    visible: true
    x: 0
    y: 0

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
Rectangle {
    id: window
    width: SCREENA_SIZEW
    height: APP_SIZEH*SCREEN_PERCENT/100
    focus: true
    color: "#272822"
    visible: true
    x: 0
    y: 100

    Material.theme: Material.Dark
    Material.accent: Material.Pink

    Layout.alignment: Qt.AlignHCenter



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
            implicitWidth: SCREENA_SIZEW*0.8
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

                contentItem: Text {
                    id: startTheGameButtonText
                    parent: startTheGameButton
                    text: startTheGameButton.text
                    font.pixelSize: 16*SIZE_FACTOR
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
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
                    easing.type: Easing.Linear
                    property int myduration: 500
                    Component.onCompleted: {
                        from = startTheGameButton.topPadding.valueOf()
                        to = startTheGameButton.height*2
                        duration = myduration.valueOf()
                    }

                    onFinished:{
                        var button = startTheGameButton
                        var animation = startTheGameButtonPaddingAnimation
                        if (button.buttonMode === "READY_WAITING"){
                            clippingRect.enabled = false
                            mainWindow.readyWaiting()
                        }
                        if (button.buttonMode === "START"){
                            console.log("yep start")
                            button.buttonMode = "STARTED"
                            startTheGameButtonText.text = "Start the game"
                            animation.from = -button.height*0.7
                            animation.to = 12*SIZE_FACTOR
                            animation.duration = animation.myduration/2
                            animation.start()
                        }
                        else if (button.buttonMode === "READY"){
                            button.buttonMode = "READY_WAITING"
                            startTheGameButtonText.text = "Waiting for other players to be ready..."
                            animation.from = -button.height*0.7
                            animation.to = 12*SIZE_FACTOR
                            animation.duration = animation.myduration/2
                            animation.start()
                        }else{
                            animation.from = 12*SIZE_FACTOR
                            animation.to = button.height*2
                            animation.duration = animation.myduration
                        }
                    }
                }

                onClicked: {
                    if (startTheGameButton.buttonMode === "STARTED"){
                        startTheGameButton.buttonMode = "STARTED_BY_US"
                        startTheGameButtonOpacityAnim.start()
                        mainWindow.gameStarted()
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
        width: SCREENA_SIZEW
        height: APP_SIZEH*SCREEN_PERCENT/100
        focus: true
        visible: true
        x: SCREENA_SIZEW
        onBet: {
            mainWindow.playerBet(amount)
            windowSlidingDownAnim.start()
        }

        onFold: {
            mainWindow.playerFold()
            windowSlidingDownAnim.start()
        }
    }

    PropertyAnimation {
        id: playPanelEntryAnim
        target: playPanel
        property: "x"
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
        Component.onCompleted: {
            from = playPanel.x.valueOf()
        }
    }

    PropertyAnimation {
        id: windowSlidingAnim
        target: window
        property: "y"
        to: 0
        easing.type: Easing.OutCubic
        duration: 1000
        Component.onCompleted: {
            from = window.y.valueOf()
        }

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
        easing.type: Easing.OutCubic
        duration: 1000
        Component.onCompleted: {
            to = window.height
        }

        onFinished: {
            if (startTheGameButton.buttonMode == "STARTED_BY_US")
                playPanelEntryAnim.start()

        }
    }
}
}
