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
	virtual void OnConstruction(const FTransform & Transform) override;

public:

	// Function when the item is being equipped
	virtual void EquipItem();
	virtual void UnEquipItem();
	virtual void UseItem();
	virtual void CollectObject(AFirstPersonCharacter* NewOwner);

	bool IsEquiped() { return bEquiped; }
	bool HasExternalDependancy() { return bExternalDependancy; }

private:

	// Condition that checks if the object is 
	bool bEquiped = false;

	// If this condition is true, this object relies on other objects in the inventory to work
	bool bExternalDependancy = false;
};
