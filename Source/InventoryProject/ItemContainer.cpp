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
	// Check if emtpy slot class was assigned - not doing this check can crash the editor
	if (EmptySlotClass)
	{
		// Spawn an empty slot actor into the world, so that we can reference it
		EmptySlot = Cast<ACollectableObject>(this->GetWorld()->SpawnActor(EmptySlotClass));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No empty slot class blueprint assigned!"));
	}
	// Check if we need to add item slots
	while (ContainerItems.Num() < MaxItemSlots)
	{
		ACollectableObject* itemSlot = EmptySlot;
		// If so, add the emtpy slot to the container
		ContainerItems.Add(EmptySlot);
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
			if(ContainerItems[i] == EmptySlot)
			{
				// Set the empty slot to equal the new item
				ContainerItems[i] = NewItem;
			}
		}
	}
}