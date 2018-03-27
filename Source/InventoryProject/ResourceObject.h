// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableObject.h"
#include "ResourceObject.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API AResourceObject : public ACollectableObject
{
	GENERATED_BODY()
	
private:
	// The number of resources that the object spawns with
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats", Meta = (AllowPrivateAccess = "true"))
	int	InitialResources = 20;

	// The number of resources that one slot can contain before a new object needs to be created
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Stats", Meta = (AllowPrivateAccess = "true"))
	int	MaxResourcesPerSlot = 20;

	// Current number of resources contained by the object
	int CurResourceCount = 0;

public:
	// Return current number of resources
	UFUNCTION(BlueprintCallable)
	int GetResourceCount() { return CurResourceCount; }

	UFUNCTION()
	virtual FString GetIndicatorName() override { return IndicatorDisplayName + " (" + FString::FromInt(CurResourceCount) + ")"; }

protected:

	virtual void Init() override;
};
