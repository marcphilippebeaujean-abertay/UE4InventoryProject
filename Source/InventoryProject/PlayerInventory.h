// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "PlayerInventory.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPROJECT_API UPlayerInventory : public UItemContainer
{
	GENERATED_BODY()
	
public: 
	// Get UI object indicator
	FString GetObjectIndicator(AActor* OtherActor);
	// Checks if object that is traced by player is of type collectable object (avoids circular dependancy between collectable and first person character)
	bool ElligableForPickup(AActor* OtherActor);
	// Collect item, add it to inventory and disable in game world
	void CollectObject(AActor* OtherActor);
};
