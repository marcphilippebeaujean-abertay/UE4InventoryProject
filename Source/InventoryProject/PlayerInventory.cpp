// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInventory.h"

void UPlayerInventory::CollectObject(ACollectableObject* OtherActor)
{
	// Disable the collectable object
	OtherActor->CollectObject();
	// Add the object to inventory
	AddObjToContainer(OtherActor);
}



