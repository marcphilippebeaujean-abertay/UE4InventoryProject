// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CollectableObject.h"
#include "FirstPersonCharacter.h"
#include "ItemContainer.h"
#include "NewPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API ANewPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

private:

	// Variable that holds the widget after it is assigned in the blue print
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> DefaultInterfaceWidgetClass;
	UPROPERTY()
	UUserWidget* DefaultInterfaceWidget = nullptr;

	// Initialise inventory components
	void InitInterfaceWidgets();
	void InitContainers();

	// Displays the bulk of the inventory - should be toggled on and off
	UWidget* InventoryWidget = nullptr;

	// Inventory containing all the objects the player collected
	UPROPERTY()
	UPlayerInventory* Inventory = nullptr;

	// Items stored in the quick access bar to 
	UPROPERTY()
	UQuickAccess* QuickAccessBar = nullptr;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

public:

	// Blueprint functions that make item containers globally accessible
	UFUNCTION(BlueprintCallable)
	UItemContainer* GetInventoryContainer() { return Inventory; }
	UFUNCTION(BlueprintCallable)
	UItemContainer* GetQuickAccessContainer() { return QuickAccessBar; }
	// Update all UI elements
};
