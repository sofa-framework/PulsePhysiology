/*
Copyright 2018, CNRS

This file is part of sofaqtquick.

sofaqtquick is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

sofaqtquick is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with sofaqtquick. If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Dialogs 1.2
import QtQuick.Window 2.2
import SofaBasics 1.0
import SofaApplication 1.0
//import SofaSceneItemModel 1.0
import SofaWidgets 1.0
import PulseWidget 1.0

RowLayout
{
    id: rowLayout

    PulseWidget {
        id: pulsewidget
    }

    TextField {
        id: scenario
        placeholderText: "Scenario"
        Layout.fillWidth: true

        text: pulsewidget.scenario
        onEditingFinished: {
            if(pulsewidget.scenario != text) pulsewidget.scenario = text
        }
    }

    Button {
        id: button
        text: "Run"
        onClicked: {
            if(pulsewidget.scenario != scenario.text) pulsewidget.scenario = scenario.text
        }
    }
}
