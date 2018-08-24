/******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, development version     *
*                (c) 2006-2018 INRIA, USTL, UJF, CNRS, MGH                    *
*                                                                             *
* This program is free software; you can redistribute it and/or modify it     *
* under the terms of the GNU Lesser General Public License as published by    *
* the Free Software Foundation; either version 2.1 of the License, or (at     *
* your option) any later version.                                             *
*                                                                             *
* This program is distributed in the hope that it will be useful, but WITHOUT *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or       *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License *
* for more details.                                                           *
*                                                                             *
* You should have received a copy of the GNU Lesser General Public License    *
* along with this program. If not, see <http://www.gnu.org/licenses/>.        *
*******************************************************************************
* Authors: The SOFA Team and external contributors (see Authors.txt)          *
*                                                                             *
* Contact information: contact@sofa-framework.org                             *
******************************************************************************/
#include <PulsePhysiology.h>

#ifdef QT_PLUGIN
#include <PulsePhysiology/PulseWidget.h>
#include <QApplication>
#include <QDebug>
#include <QQuickPaintedItem>

const int versionMajor = 1;
const int versionMinor = 0;

static void initResources()
{
    Q_INIT_RESOURCE(PulseWidget_qrc);
}
#endif // QT_PLUGIN

namespace sofa
{
namespace pulsephysiology
{

extern "C" {
    SOFA_PULSEPHYSIOLOGY_API void initExternalModule();
    SOFA_PULSEPHYSIOLOGY_API const char* getModuleName();
    SOFA_PULSEPHYSIOLOGY_API const char* getModuleVersion();
    SOFA_PULSEPHYSIOLOGY_API const char* getModuleLicense();
    SOFA_PULSEPHYSIOLOGY_API const char* getModuleDescription();
    SOFA_PULSEPHYSIOLOGY_API const char* getModuleComponentList();
}

void initExternalModule()
{
    static bool first = true;
    if (first)
    {
        first = false;
    }
#ifdef QT_PLUGIN
    initResources();

    qmlRegisterType<sofa::pulsephysiology::PulseWidget>("PulseWidget", versionMajor, versionMinor, "PulseWidget");
#endif // QT_PLUGIN
}

const char* getModuleName()
{
    return "PulsePhysiology";
}

const char* getModuleVersion()
{
    return "1.0";
}

const char* getModuleLicense()
{
    return "LGPL";
}

const char* getModuleDescription()
{
    return getModuleName();
}

const char* getModuleComponentList()
{
    return "";
}

} // namespace pulsephysiology
} // namespace sofa
