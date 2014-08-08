import QtQuick 2.0
import Sailfish.Silica 1.0


Page {
    id: page

    property var lastUsedButton: 0

    function sendCommand(handle, cmd)
    {
        lastUsedButton = handle;
        ipsmanager.sendCommand(cmd);
    }

    function updateColor() {
        switch(ipsmanager.progress) {
            case 0 /* IDLE */:    status.color = Theme.primaryColor; break;
            case 1 /* ACTIVE */:  status.color = "yellow"; break;
            case 2 /* ERROR */:   status.color = "red"; colorTimer.start(); break;
            case 3 /* SUCCESS */: status.color = "lime"; colorTimer.start(); break;
        }
    }

    Component.onCompleted: {
        ipsmanager.progressChanged.connect(updateColor);
    }

    Timer {
        id: colorTimer
        interval: 5000
        onTriggered: {
            sw1on.color = Theme.primaryColor;
            sw1off.color = Theme.primaryColor;
            sw2on.color = Theme.primaryColor;
            sw2off.color = Theme.primaryColor;
            bothon.color = Theme.primaryColor;
            bothontimed.color = Theme.primaryColor;
            bothoff.color = Theme.primaryColor;
            bothoff2min.color = Theme.primaryColor;
            bothoff30min.color = Theme.primaryColor;
        }

        repeat: false
    }

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsDialog.qml"))
            }
        }

        contentHeight: column.height

        Column {
            id: column

            width: page.width
            spacing: Theme.paddingSmall

            PageHeader {
                title: qsTr("IPSteckdose")
            }

            Label {
                id: status
                anchors.horizontalCenter: parent.horizontalCenter
                text: ipsmanager.status
                font.bold: true
            }

            Column {
                id: actionPart

                width: page.width
                spacing: Theme.paddingSmall

                visible: (ipsmanager.progress != 1)
                enabled: (ipsmanager.progress != 1)

                SectionHeader {
                    text: qsTr("Socket 1")
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: sw1on
                        text: qsTr("Switch On")
                        onClicked: sendCommand(sw1on, '1')
                    }
                    Button {
                        id: sw1off
                        text: qsTr("Switch Off")
                        onClicked: sendCommand(sw1off, '2')
                    }
                }
                SectionHeader {
                    text: qsTr("Socket 2")
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: sw2on
                        text: qsTr("Switch On")
                        onClicked: sendCommand(sw2on, '3')
                    }
                    Button {
                        id: sw2off
                        text: qsTr("Switch Off")
                        onClicked: sendCommand(sw2off, '4')
                    }
                }
                SectionHeader {
                    text: qsTr("Both sockets")
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: bothon
                        text: qsTr("Switch On")
                        onClicked: sendCommand(bothon, '5')
                    }
                    Button {
                        id: bothoff
                        text: qsTr("Switch Off")
                        onClicked: sendCommand(bothoff, '6')
                    }
                }
                Button {
                    id: bothontimed
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: qsTr("On with delay")
                    onClicked: sendCommand(bothontimed, '7')
                }
                Row {
                    anchors.horizontalCenter: parent.horizontalCenter

                    Button {
                        id: bothoff2min
                        text: qsTr("Off [2 minutes]")
                        onClicked: sendCommand(bothoff2min, '8')
                    }
                    Button {
                        id: bothoff30min
                        text: qsTr("Off [30 minutes]")
                        onClicked: sendCommand(bothoff30min, '9')
                    }
                }
            }
            Column {
                id: cancelPart

                width: page.width
                spacing: Theme.paddingSmall

                visible: (ipsmanager.progress == 1)
                enabled: (ipsmanager.progress == 1)

                SectionHeader {
                    text: qsTr("Action")
                }
                Button {
                    id: cancel
                    anchors.horizontalCenter: parent.horizontalCenter

                    text: qsTr("Cancel")
                    onClicked: ipsmanager.cancelRequest()
                }
            }
        }
    }
}


