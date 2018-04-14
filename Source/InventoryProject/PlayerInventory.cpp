// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"

void UPlayerInventory::BeginPlay()
{
	Super::BeginPlay();
	// Get the owning player of the inventory
	OwningPlayer = Cast<AFirstPersonCharacter>(GetOwner());
	if(!OwningPlayer)
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is missing owning player!"));
	}
}

void UPlayerInventory::CollectObject(ACollectableObject* OtherActor)
{
	// Add the object to inventory
	AddObjToContainer(OtherActor);
}
