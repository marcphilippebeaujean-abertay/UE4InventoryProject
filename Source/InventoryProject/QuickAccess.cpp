// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickAccess.h"


// Sets default values for this component's properties
void UQuickAccess::OnComponentCreated()
{
	Super::OnComponentCreated();

	// Set quick access check to true
	bQuickAccess = true;
	// Make sure rows is set to 1
	NumberOfRows = 1;
	// Set collecte item
	CurSelectedItem = 0;
}

void UQuickAccess::UpdateSelectedItem(bool increment)
{
	// Unequip the current item
	Cast<AEquipableObject>(GetContainerItem(CurSelectedItem))->UnEquipItem();
	// Manipulate selected value item based on condition
	if(increment)
	{
		CurSelectedItem++;
	}
	else
	{
		CurSelectedItem--;
	}
	// Ensure that cur selected item is going to be within the container's array - if they exceed them, return the highest/lowest possible value
	if(CurSelectedItem > (GetNumOfCollumns() - 1))
	{
		CurSelectedItem = 0;
	}
	if(CurSelectedItem < 0)
	{
		CurSelectedItem = (GetNumOfCollumns() - 1);
	}
	// Equip the new current item
	Cast<AEquipableObject>(GetContainerItem(CurSelectedItem))->EquipItem();
	BroadcastWidgetUpdate();
}

void UQuickAccess::SetContainerItem(int ContainerID, ACollectableObject* NewItem)
{
	Super::SetContainerItem(ContainerID, NewItem);
	// Check if the new object is being assigned as the current item slot
	if (ContainerID == CurSelectedItem)
	{
		// Access the equipable type
		AEquipableObject* EquipObj = Cast<AEquipableObject>(NewItem);
		// Check if cast was successful
		if (EquipObj == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("Failed cast to equipable object - cannot equip!"));
			return;
		}
		// See if item is equipable
		EquipObj->EquipItem();
	}
}

void UQuickAccess::SetContainerItems(TArray<ACollectableObject*> l_containerItems)
{
	Super::SetContainerItems(l_containerItems);
	// Access the equipable type
	AEquipableObject* EquipObj = Cast<AEquipableObject>(ContainerItems[CurSelectedItem]);
	// Check if cast was successful
	if (EquipObj == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed cast to equipable object - cannot equip!"));
		return;
	}
	// See if item is equipable
	EquipObj->EquipItem();
}

void UQuickAccess::SwapItems(UItemContainer* OtherContainer, int OtherItemID, int LocalItemID)
{
	// Check if other item is equipable (otherwise, no need to try for swap)
	if (Cast<AEquipableObject>(OtherContainer->GetContainerItem(OtherItemID)))
	{
		// If required, unequip item from quick access bar
		CheckForUnequip(LocalItemID);
		Super::SwapItems(OtherContainer, OtherItemID, LocalItemID);
	}
}

void UQuickAccess::CheckForUnequip(int ItemID)
{
	// See if item ID is currently equiped item
	if(ItemID == CurSelectedItem)
	{
		// Unequip item
		Cast<AEquipableObject>(GetContainerItem(CurSelectedItem))->UnEquipItem();
	}
}

void UQuickAccess::DropItem(int ItemID)
{
	// Unequip item before dropping it
	if(ItemID == CurSelectedItem)
	{
		if(AEquipableObject* CurItem = Cast<AEquipableObject>(GetContainerItem(ItemID)))
		{
			CurItem->UnEquipItem();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed cast to equipable object while dropping!"));
			return;
		}
	}
	Super::DropItem(ItemID);
}
