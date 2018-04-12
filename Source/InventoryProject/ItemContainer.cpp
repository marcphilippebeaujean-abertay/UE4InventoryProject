// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemContainer.h"


// Sets default values for this component's properties
UItemContainer::UItemContainer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemContainer::BeginPlay()
{
	Super::BeginPlay();

}

void UItemContainer::InitContainerContents(ACollectableObject* EmptySlotClass)
{
	// Set empty slot class
	EmptySlot = EmptySlotClass;
	// Check if emtpy slot class was assigned - not doing this check can crash the editor
	if (EmptySlot != nullptr)
	{
		// Check if we need to add item slots
		for (int i = 0; i < MaxItemSlots; i++)
		{
			// If so, add the emtpy slot to the container
			ContainerItems.Add(EmptySlot);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No empty slot class blueprint assigned!"));
	}
}

// Called every frame
void UItemContainer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemContainer::AddObjToContainer(ACollectableObject* NewItem)
{
	// Check if there is space in the container
	if(ContainerItems.Num() > 0)
	{
		// Iterate through each slot until an empty slot is found
		for(int i = 0; i < ContainerItems.Num(); i++)
		{
			if(ContainerItems[i]->IsEmptySlot())
			{
				// Set the empty slot to equal the new item
				ContainerItems[i] = NewItem;
				// Update the widget
				BroadcastWidgetUpdate();
				// If we found a slot, only add one instance of the item to the container
				break;
			}
		}
	}
}

void UItemContainer::SwapItems(UItemContainer* OtherContainer, int OtherItemID, int LocalItemID)
{
	// Check if its possible to swap the items - first check if one of the items is quick access
	if(bQuickAccess || OtherContainer->IsQuickAccess())
	{
		// Check if both items are equipable
		if(!GetContainerItem(LocalItemID)->IsEquipable() || !OtherContainer->GetContainerItem(OtherItemID)->IsEquipable())
		{
			// If not, don't perform swap
			return;
		}
	}
	// Create temporary object that stores values from other container's item
	ACollectableObject* TempObject = OtherContainer->GetContainerItem(OtherItemID);
	// Set other containers item to be able equal to that of the local one
	OtherContainer->SetContainerItem(OtherItemID, GetContainerItem(LocalItemID));
	// Set local item to be equal to that of the temporary object
	SetContainerItem(LocalItemID, TempObject);
	
	// Update widgets
	BroadcastWidgetUpdate();
	OtherContainer->BroadcastWidgetUpdate();
}

void UItemContainer::BroadcastWidgetUpdate()
{
	OnUpdateContainer.Broadcast(ContainerItems);
}
