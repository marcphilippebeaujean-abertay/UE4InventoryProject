// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemContainer.h"
#include "QuickAccess.h"

// Sets default values for this component's properties
UItemContainer::UItemContainer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

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
	// Fill container with the needed number of empty slots
	ContainerItems.Init(EmptySlotClass, GetMaxItemSlots());
	// Check objects to be initialised isnt too big
	if(InitItems.Num() > GetMaxItemSlots())
	{
		UE_LOG(LogTemp, Error, TEXT("More init items assigned than can be spawned!"));
	}
	else
	{
		// Spawn items that should be in the container on initialisation
		for (int i = 0; i < InitItems.Num(); i++)
		{
			// Create item reference
			ACollectableObject* NewItem = Cast<ACollectableObject>(GetWorld()->SpawnActor(InitItems[i]));
			// Add the item to the container
			AddObjToContainer(NewItem);
		}
	}
	// Set condition for initialisation to true
	bUninitialised = false;
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
				// Collect item
				ContainerItems[i]->OnObjectCollected(this);
				// Check for depleted items
				CheckForDepletedItems();
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
		// Check if other object is the quick access bar
		if(OtherContainer->IsQuickAccess())
		{
			if(UQuickAccess* OtherQuickAccess =  Cast<UQuickAccess>(OtherContainer))
			{
				// Check if the item needs to be unequiped
				OtherQuickAccess->CheckForUnequip(OtherItemID);
			}
		}
	}
	// Create temporary object that stores values from other container's item
	ACollectableObject* TempObject = OtherContainer->GetContainerItem(OtherItemID);
	// Update owners for the object and auto stack
	TempObject->UpdateObjectOwner(this);
	// Check if the object dragged onto this container has been depleted by auto stacking
	if (TempObject->GetCurItemsInSlot() > 0)
	{
		GetContainerItem(LocalItemID)->UpdateObjectOwner(OtherContainer);
		// Set other containers item to be able equal to that of the local one
		OtherContainer->SetContainerItem(OtherItemID, GetContainerItem(LocalItemID));
		// Set local item to be equal to that of the temporary object
		SetContainerItem(LocalItemID, TempObject);
	}
	else
	{
		// Its not a swap, since the object dragged to the new container has been depleted - replace object in other container with empty slot
		OtherContainer->SetContainerItem(OtherItemID, OtherContainer->GetEmptySlot());
	}
	// Update widgets
	BroadcastWidgetUpdate();
	OtherContainer->BroadcastWidgetUpdate();
}

void UItemContainer::BroadcastWidgetUpdate()
{
	OnUpdateContainer.Broadcast(ContainerItems);
}

ACollectableObject* UItemContainer::GetResourceOfType(EResourceType ResType)
{
	// Initialise collectable reference
	ACollectableObject* ReturnItem = nullptr;
	// Search inventory for object that has the required resource
	for (int i = 0; i < ContainerItems.Num(); i++)
	{
		// Check if the current item contains the required resource type
		if (ContainerItems[i]->GetItemResourceType() == ResType)
		{
			// Check if return item has been assigned
			if (ReturnItem == nullptr)
			{
				// ..if not, assign this new item
				ReturnItem = ContainerItems[i];
			}
			else
			{
				// Check if this new item is storing less items than the other
				if(ContainerItems[i]->GetCurItemsInSlot() < ReturnItem->GetCurItemsInSlot())
				{
					// We always want to return the item with the lowest amount of resources and subtract from that
					ReturnItem = ContainerItems[i];
				}
			}
		}
	}
	return ReturnItem;
}

void UItemContainer::CheckForDepletedItems()
{
	// Check each item in array
	for(int i = 0; i < ContainerItems.Num(); i++)
	{
		if(ContainerItems[i]->GetCurItemsInSlot() <= 0)
		{
			// this should now be an empty slot!
			ContainerItems[i] = EmptySlot;
		}
	}
}

void UItemContainer::DropItem(int ItemID)
{
	// Check if object owner is the player
	if(AFirstPersonCharacter* Player = Cast<AFirstPersonCharacter>(GetOwner()))
	{
		// Drop the object
		ContainerItems[ItemID]->DropItem(Player->GetRayEndPoint(false));
		// Remove it from the array
		ContainerItems.RemoveAt(ItemID);
		// Insert a reference to the empty slot
		ContainerItems.Insert(EmptySlot, ItemID);
		// Broadcast a widget update
		BroadcastWidgetUpdate();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Owning actor was not a player!"));
	}
}

TArray<ACollectableObject*> UItemContainer::GetResourcesOfType(EResourceType ResType)
{
	TArray<ACollectableObject*> ResourceObjects;
	// Loop through all items
	for(int i = 0; i < ContainerItems.Num(); i++)
	{
		// Object of that type is found..
		if(ContainerItems[i]->GetItemResourceType() == ResType)
		{
			// ..retrieve it and add it to the array!
			ResourceObjects.Add(ContainerItems[i]);
		}
	}
	return ResourceObjects;
}
