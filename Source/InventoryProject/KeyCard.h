// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableObject.h"
#include "KeyCard.generated.h"

/**
 * 
 */
// Defines that doors that a given key card can unlock
UENUM(BlueprintType)
enum class EKeyType : uint8
{
	Master,
};

UCLASS()
class INVENTORYPROJECT_API AKeyCard : public ACollectableObject
{
	GENERATED_BODY()
	
private:

	// Variable used to determine the doors that this key can unlock
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Key Unlock", Meta = (AllowPrivateAccess = "true"))
	EKeyType KeyTarget = EKeyType::Master;
	
public:

	EKeyType GetKeyType();
	
};
