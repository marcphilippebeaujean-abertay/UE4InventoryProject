// Fill out your copyright notice in the Description page of Project Settings.

#include "MySaveGame.h"
#include "ItemContainer.h"


UMySaveGame::UMySaveGame()
{
	m_saveSlotName = TEXT("TestSaveSlot");
	m_userIndex = 0;
}

void UMySaveGame::SaveContainerItems(FString l_containerID, UItemContainer* l_itemContainer)
{
	// Check if map for the container already exists
	if (m_containerItems.Find(l_containerID))
	{
		// Clear the item refernces
		m_containerItems[l_containerID].Empty();
	}
	// Iterate through the container reference
	uint32 itemIndex = 0;
	for(auto &itr : l_itemContainer->GetContainerItems())
	{
		// Create new game object storage struct
		FInventoryObjectsStruct inventoryItem;
		// Add each item's blueprint class and assign required varaibles
		inventoryItem.SetUnitCount(itr->GetCurUnitsInSlot());
		inventoryItem.SetInventoryIndex(itemIndex);
		inventoryItem.SetCollectableClass(itr->GetClass());
		// Increment item index iterator
		itemIndex++;
	}
}
