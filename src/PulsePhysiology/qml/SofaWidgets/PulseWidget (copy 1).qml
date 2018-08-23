import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import SofaBasics 1.0
import SofaApplication 1.0
import SofaData 1.0
import SofaWidgets 1.0
import QtWebSockets 1.0
import PulseWidget 1.0

ColumnLayout {
    id: root
    spacing: 0
    enabled: sofaScene ? sofaScene.ready : false

    height: columnLayout.implicitHeight

    //implicitWidth: 300

    function appendMessage(message) {
        messageBox.text += "\n" + message
    }

    property var sofaScene: SofaApplication.sofaScene
    property var sofaComponent: sofaScene ? sofaScene.selectedComponent: null
    Item {
        id: scrollView
        Layout.fillWidth: true
        Layout.preferredHeight: 120

        GridLayout {
            id: gridLayout
            width: scrollView.width - 9
            columns: 2


            PulseWidget {
                id: webserver
                Component.onCompleted: sofaScene = SofaApplication.sofaScene;//componentComplete();//root.sofaScene.animate = true; //
                property bool sceneReady: root.sofaScene && root.sofaScene.ready
            }

            Label {
                Layout.preferredWidth: implicitWidth
                text: "IP Address"
            }
            Rectangle {
                color: "white"
                TextInput {
                    id: ipSpinBox
                    Layout.columnSpan: 2
                    Layout.fillWidth: true

                
                    //validator: DoubleValidator {bottom: 0 }
                    validator: RegExpValidator {
                        regExp: /^((?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])\.){0,3}(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])$/
                    }
                    inputMask: "000.000.000.000;0"

                    text: "192.168.1.1"
                    //text: root.sofaScene ? root.sofaScene.dt.toString() : Number(0.04).toString()

                    onAccepted: {
                        if (0 === Number(text))
                            text = "Real-time";
                    }
                }
                width: childrenRect.width
                height: childrenRect.height
            }


            Label {
                Layout.preferredWidth: implicitWidth
                text: "Port Address"
            }

            Rectangle {
                color: "white"
                TextInput {
                    id: portSpinBox
                    Layout.columnSpan: 2
                    Layout.fillWidth: true

                    //validator: DoubleValidator {bottom: 0 }
                    validator: IntValidator {bottom: 0 }
                    //inputMask: "0000;0"

                    text: webserver.port
                    //text: root.sofaScene ? root.sofaScene.dt.toString() : Number(0.04).toString()
                    onTextChanged: webserver.port = Number(text)

                    onAccepted: {
                    }
                }
                width: childrenRect.width
                height: childrenRect.height
            }


            Label {
                Layout.columnSpan: 1
                Layout.fillHeight: true
                //Layout.preferredWidth: implicitWidth
                text: "STATUS"
            }

            ScrollView {
                Layout.fillHeight: true
                //height: parent.height
                horizontalScrollBarPolicy: Qt.ScrollBarOn
                Rectangle {
                    color: "white"
                    height: parent.height
                    Label {
                        id: statusSpinBox

                        Layout.columnSpan: 2
                        Layout.fillWidth: true
                    
                        text: webserver.status
                    }

                }
            }
        }
    }
}
