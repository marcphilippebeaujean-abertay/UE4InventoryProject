// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableObject.h"
#include "EquipableObject.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class INVENTORYPROJECT_API AEquipableObject : public ACollectableObject
{
	GENERATED_BODY()
	
protected:

	// Since overloading doesn't work in unreal, use OnConstruction instead
	virtual void Init() override;
};
