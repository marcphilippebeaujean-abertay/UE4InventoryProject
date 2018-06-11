// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "KeyCard.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API ADoor : public AInteractable
{
	GENERATED_BODY()
	
public:

	virtual void OnInteracted() override;
	virtual bool CanInteract() override;

private:

	// Variable used to determine what type of key can open the door
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door", Meta = (AllowPrivateAccess = "true"))
	EKeyType RequiredKey = EKeyType::Master;

	// Character reference
	UPROPERTY()
	class AFirstPersonCharacter* PlayerRef = nullptr;
};
