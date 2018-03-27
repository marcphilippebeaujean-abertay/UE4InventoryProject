// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"

FString UPlayerInventory::GetObjectIndicator(AActor* OtherActor)
{
	// Check if other actor is of type collectable
	if (ACollectableObject* IndicatorCollectable = Cast<ACollectableObject>(OtherActor))
	{
		// Return the name of the collectable
		return IndicatorCollectable->GetIndicatorName();
	}
	// Otherwise, return nothing
	return "";
}

bool UPlayerInventory::ElligableForPickup(AActor* OtherActor)
{
	// Check if other actor is of type collectable
	if (ACollectableObject* IndicatorCollectable = Cast<ACollectableObject>(OtherActor))
	{
		return true;
	}
	return false;
}

void UPlayerInventory::CollectObject(AActor* OtherActor)
{
	// Cast to collectable object
	ACollectableObject* ActorToCollect = Cast<ACollectableObject>(OtherActor);
	// Disable the collectable object
	ActorToCollect->CollectObject();
	// Add the object to inventory
	AddObjToContainer(ActorToCollect);
}



