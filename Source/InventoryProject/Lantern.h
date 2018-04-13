// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipableObject.h"
#include "LanternItemComponent.h"
#include "Lantern.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API ALantern : public AEquipableObject
{
	GENERATED_BODY()

private:
	// Lantern component attached to the player
	ULanternItemComponent* LanternComponent = nullptr;
	
public:
	// Override object specific functions
	virtual void EquipItem() override;
	virtual void UnEquipItem() override;
	virtual void CollectObject(AFirstPersonCharacter* NewOwner) override;
	virtual void UseItem() override;
};
