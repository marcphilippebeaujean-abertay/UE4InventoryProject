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
	if (ContainerIsStored(l_containerID))
	{
		// Clear the item refernces
		m_containerItems[l_containerID].Empty();
	}
	else
	{
		// Otherwise, create new instance of the map
		TArray<FInventoryObjectsStruct> newArray;
		m_containerItems.Add(l_containerID, newArray);
	}
	// Iterate through the container reference
	uint32 itemIndex = 0;
	for(auto &itr : l_itemContainer->GetContainerItems())
	{
		// Make sure its not an empty slot
		if (itr->IsEmptySlot())
		{
			// Create new game object storage struct
			FInventoryObjectsStruct inventoryItem;
			// Add each item's blueprint class and assign required varaibles
			inventoryItem.SetUnitCount(itr->GetCurUnitsInSlot());
			inventoryItem.SetInventoryIndex(itemIndex);
			inventoryItem.SetCollectableClass(itr->GetClass());
			// Increment item index iterator
			itemIndex++;
			// Append the struct to our list, with the key
			m_containerItems[l_containerID].Add(inventoryItem);
		}
	}
}

bool UMySaveGame::ContainerIsStored(FString l_containerID)
{
	if(m_containerItems.Find(l_containerID) == nullptr)
	{
		return false;
	}
	return true;
}


TArray<FInventoryObjectsStruct> UMySaveGame::LoadContainerItems(FString l_containerID)
{
	return m_containerItems[l_containerID];
}