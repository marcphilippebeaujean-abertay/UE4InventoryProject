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
		m_containerRefs[l_containerID].m_containerItems.Empty();
	}
	else
	{
		// Otherwise, create new instance of the map
		FContainerReference newContainerRef;
		newContainerRef.m_containerID = l_containerID;
		newContainerRef.m_containerItems = TArray<FContainerObjectReference>();
		m_containerRefs.Add(l_containerID, newContainerRef);
		if(m_containerRefs.Num() <= 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to add container to map!"));
		}
	}
	// Iterate through the container reference
	uint32 itemIndex = 0;
	for(auto &itr : l_itemContainer->GetContainerItems())
	{
		// Create new game object storage struct
		FContainerObjectReference inventoryItem;
		// Add each item's blueprint class and assign required varaibles
		inventoryItem.SetUnitCount(itr->GetCurUnitsInSlot());
		inventoryItem.SetInventoryIndex(itemIndex);
		inventoryItem.SetCollectableClass(itr->GetClass());
		// Increment item index iterator
		itemIndex++;
		// Append the struct to our list, with the key
		m_containerRefs[l_containerID].m_containerItems.Add(inventoryItem);
	}
}

bool UMySaveGame::ContainerIsStored(FString l_containerID)
{
	if(m_containerRefs.Find(l_containerID) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find container in map!"));
		return false;
	}
	return true;
}


TArray<FContainerObjectReference> UMySaveGame::LoadContainerItems(FString l_containerID)
{
	return m_containerRefs[l_containerID].m_containerItems;
}