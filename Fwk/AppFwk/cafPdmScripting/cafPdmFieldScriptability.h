//##################################################################################################
//
//   Custom Visualization Core library
//   Copyright (C) Ceetron Solutions AS
//
//   This library may be used under the terms of either the GNU General Public License or
//   the GNU Lesser General Public License as follows:
//
//   GNU General Public License Usage
//   This library is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU General Public License at <<http://www.gnu.org/licenses/gpl.html>>
//   for more details.
//
//   GNU Lesser General Public License Usage
//   This library is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Lesser General Public License as published by
//   the Free Software Foundation; either version 2.1 of the License, or
//   (at your option) any later version.
//
//   This library is distributed in the hope that it will be useful, but WITHOUT ANY
//   WARRANTY; without even the implied warranty of MERCHANTABILITY or
//   FITNESS FOR A PARTICULAR PURPOSE.
//
//   See the GNU Lesser General Public License at <<http://www.gnu.org/licenses/lgpl-2.1.html>>
//   for more details.
//
//##################################################################################################
#pragma once

#include <QString>
#include "cafPdmFieldCapability.h"

class QTextStream;

namespace caf {

class PdmFieldHandle;
class PdmObjectFactory;
class PdmScriptIOMessages;

#define CAF_PDM_InitScriptableField( field, keyword, default, uiName, iconResourceName, toolTip, whatsThis ) \
    CAF_PDM_InitField( field,                                                                   \
                       keyword,                                                                 \
                       default,                                                                 \
                       uiName,                                                                  \
                       iconResourceName,                                                        \
                       caf::PdmPythonGenerator::pythonHelpString( toolTip, keyword ),           \
                       whatsThis );                                                             \
    caf::PdmFieldScriptability::addToField( field, keyword )

#define CAF_PDM_InitScriptableFieldNoDefault( field, keyword, uiName, iconResourceName, toolTip, whatsThis ) \
    CAF_PDM_InitFieldNoDefault( field,                                                          \
                                keyword,                                                        \
                                uiName,                                                         \
                                iconResourceName,                                               \
                                caf::PdmPythonGenerator::pythonHelpString( toolTip, keyword ),  \
                                whatsThis );                                                    \
    caf::PdmFieldScriptability::addToField( field, keyword )

#define CAF_PDM_InitScriptableFieldWithKeyword( field, keyword, scriptKeyword, default, uiName, iconResourceName, toolTip, whatsThis ) \
    CAF_PDM_InitField( field,                                                                                            \
                       keyword,                                                                                          \
                       default,                                                                                          \
                       uiName,                                                                                           \
                       iconResourceName,                                                                                 \
                       caf::PdmPythonGenerator::pythonHelpString( toolTip, scriptKeyword ),                              \
                       whatsThis );                                                                                      \
    caf::PdmFieldScriptability::addToField( field, scriptKeyword )

#define CAF_PDM_InitScriptableFieldWithKeywordNoDefault( field, keyword, scriptKeyword, uiName, iconResourceName, toolTip, whatsThis ) \
    CAF_PDM_InitFieldNoDefault( field,                                                                                   \
                                keyword,                                                                                 \
                                uiName,                                                                                  \
                                iconResourceName,                                                                        \
                                caf::PdmPythonGenerator::pythonHelpString( toolTip, scriptKeyword ),                     \
                                whatsThis );                                                                             \
    caf::PdmFieldScriptability::addToField( field, scriptKeyword )

class PdmFieldScriptability : public PdmFieldCapability
{
public:
    PdmFieldScriptability(caf::PdmFieldHandle* owner, const QString& scriptFieldName, bool giveOwnership);
    virtual ~PdmFieldScriptability();

    const QString scriptFieldName() const;

    bool isIOWriteable() const;
    void setIOWriteable(bool writeable);

    virtual void readFromField(QTextStream& outputStream, bool quoteStrings = true, bool quoteNonBuiltins = false) const;
    virtual void writeToField(QTextStream& inputStream,
        caf::PdmObjectFactory* objectFactory,
        caf::PdmScriptIOMessages* errorMessageContainer,
        bool                      stringsAreQuoted = true);

    static void addToField(caf::PdmFieldHandle* field, const QString& fieldName);
private:
    caf::PdmFieldHandle* m_owner;
    QString              m_scriptFieldName;
    bool                 m_IOWriteable;
};


}
