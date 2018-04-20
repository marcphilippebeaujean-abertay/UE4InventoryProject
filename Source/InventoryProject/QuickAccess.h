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
	void UpdateSelectedItem(bool increment);

	// Since overloading doesn't work in unreal, use OnComponentCreated instead
	virtual void OnComponentCreated() override;

	// Returns selected item, required for item updates
	UFUNCTION(BlueprintCallable)
	int GetCurSelectedID() { return CurSelectedItem; }

	// Returns the item that is currently equiped by the player
	AEquipableObject* GetSelectedItem() { return Cast<AEquipableObject>(ContainerItems[CurSelectedItem]); }

	// Override set container item, since we need to check if an object is being equiped when a container update is made
	virtual void SetContainerItem(int ContainerID, ACollectableObject* NewItem) override;
	virtual void SwapItems(UItemContainer* OtherContainer, int OtherItemID, int LocalItemID) override;
	void CheckForUnequip(int ItemID);
};
