// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "FirstPersonCharacter.h"
#include "PlayerInventory.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPROJECT_API UPlayerInventory : public UItemContainer
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

private:

public: 
	// Collect item, add it to inventory and disable in game world
	void CollectObject(ACollectableObject* OtherActor);
};
