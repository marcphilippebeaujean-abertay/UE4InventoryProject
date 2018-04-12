// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EquipableObject.h"
#include "DefaultEmptySlot.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API ADefaultEmptySlot : public AEquipableObject
{
	GENERATED_BODY()
	
protected:
	// Constructor to define empty slot
	void OnConstruction(const FTransform & Transform) override;

public:

	virtual void EquipItem() override;
};
