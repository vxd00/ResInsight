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

#include "RicDeleteItemsInCollectionFeature.h"
#include "RicDeletableItemCollection.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmObject.h"
#include "cafPdmUiItem.h"
#include "cafSelectionManager.h"

#include <QAction>

CAF_CMD_SOURCE_INIT(RicDeleteItemsInCollectionFeature, "RicDeleteItemsInCollectionFeature");

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RicDeleteItemsInCollectionFeature::isCommandEnabled()
{
    std::vector<caf::PdmUiItem*> items;
    caf::SelectionManager::instance()->selectedItems(items);

    if (items.empty()) return false;

    for (caf::PdmUiItem* item : items)
    {
        if (!RicDeleteItemsInCollectionFeature::deletableItemCollection(item))
        {
            return false;
        }
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicDeleteItemsInCollectionFeature::onActionTriggered(bool isChecked)
{
    std::vector<caf::PdmUiItem*> items;
    caf::SelectionManager::instance()->selectedItems(items);

    std::set<RicDeletableItemCollection*> collToUpdate;

    for (caf::PdmUiItem* item : items)
    {
        caf::PdmObject* currentPdmObject = dynamic_cast<caf::PdmObject*>(item);
        if (currentPdmObject)
        {
            RicDeletableItemCollection* itemColl = RicDeleteItemsInCollectionFeature::deletableItemCollection(item);
            if (itemColl)
            {
                itemColl->deleteObject(currentPdmObject);

                collToUpdate.insert(itemColl);
            }
        }
    }

    for (auto c : collToUpdate)
    {
        c->updateAfterDeleteOfItems();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RicDeleteItemsInCollectionFeature::setupActionLook(QAction* actionToSetup)
{
    actionToSetup->setText("Delete");
    actionToSetup->setIcon(QIcon(":/Erase.png"));
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RicDeletableItemCollection* RicDeleteItemsInCollectionFeature::deletableItemCollection(caf::PdmUiItem* item)
{
    caf::PdmObject* currentPdmObject = dynamic_cast<caf::PdmObject*>(item);
    if (currentPdmObject)
    {
        if (currentPdmObject->parentField())
        {
            RicDeletableItemCollection* itemColl =
                dynamic_cast<RicDeletableItemCollection*>(currentPdmObject->parentField()->ownerObject());

            return itemColl;
        }
    }

    return nullptr;
}
