// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
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
};
