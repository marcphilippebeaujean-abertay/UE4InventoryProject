// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemContainer.h"
#include "QuickAccess.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class INVENTORYPROJECT_API UQuickAccess : public UItemContainer
{
	GENERATED_BODY()

private: 
	// Item index in the array that is currently equiped by the player
	int CurSelectedItem = 0;

public:

	// Increments/Decrements the item in the action bar that is currently selected
	ACollectableObject* UpdateSelectedItem(bool increment);

	// Since overloading doesn't work in unreal, use OnComponentCreated instead
	virtual void OnComponentCreated() override;
};
