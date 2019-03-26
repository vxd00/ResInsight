/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#include "RicDeleteAllItemsInCollectionFeature.h"
#include "RicDeletableItemCollection.h"

#include "cafPdmUiItem.h"
#include "cafSelectionManager.h"

#include <QAction>

CAF_CMD_SOURCE_INIT(RicDeleteAllItemsInCollectionFeature, "RicDeleteAllItemsInCollectionFeature");

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicDeleteAllItemsInCollectionFeature::isCommandEnabled()
{
    caf::PdmUiItem* selectedItem = caf::SelectionManager::instance()->selectedItem();

    RicDeletableItemCollection* deletableItemCollection = dynamic_cast<RicDeletableItemCollection*>(selectedItem);

    if (deletableItemCollection)
    {
        return true;
    }

    return false;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicDeleteAllItemsInCollectionFeature::onActionTriggered(bool isChecked)
{
    caf::PdmUiItem* selectedItem = caf::SelectionManager::instance()->selectedItem();

    RicDeletableItemCollection* deletableItemCollection = dynamic_cast<RicDeletableItemCollection*>(selectedItem);

    if (deletableItemCollection)
    {
        deletableItemCollection->deleteAllSubItems();
        deletableItemCollection->updateAfterDeleteOfItems();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicDeleteAllItemsInCollectionFeature::setupActionLook(QAction* actionToSetup)
{
    actionToSetup->setText("Delete All Sub Items");

    actionToSetup->setIcon(QIcon(":/ToggleOn16x16.png"));
}
