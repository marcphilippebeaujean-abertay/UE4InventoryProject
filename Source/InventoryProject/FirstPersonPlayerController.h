// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FirstPersonCharacter.h"
#include "ItemContainer.h"
#include "DefaultEmptySlot.h"
#include "PlayerInventory.h"
#include "QuickAccess.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "FirstPersonPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API AFirstPersonPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	// Called to bind functionality to input
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

	AFirstPersonPlayerController();
private:
	// Variable that holds the widget after it is assigned in the blue print
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> DefaultInterfaceWidgetClass = nullptr;
	UUserWidget* DefaultInterfaceWidget = nullptr;

	// Empty slot initialised to be used by the containers
	ACollectableObject* EmptySlot = nullptr;
	
	// Displays the bulk of the inventory - should be toggled on and off
	UWidget* InventoryWidget = nullptr;

	// Mesh of the currently equiped item
	UStaticMeshComponent* ItemMesh;

	// Inventory containing all the objects the player collected
	UPROPERTY()
	UPlayerInventory* Inventory = nullptr;

	// Items stored in the quick access bar to 
	UPROPERTY()
	UQuickAccess* QuickAccessBar = nullptr;

	// Initialise inventory components
	void InitInterfaceWidgets();
	void InitContainers();

	// Bool that is toggled to enable/disable inventory
	bool bInventoryOpen = false;

	// Turn off or enable inventory
	void ToggleInventory();

	// Reference to the player character
	AFirstPersonCharacter* PlayerCharacter = nullptr;

	/// Functions called to interact with objects in the world
	void GrabObject();
	void OnGrabReleased();

	// Back-and forwards movement
	void MovementForward(float val);
	void MovementRight(float val);

public:
	UFUNCTION(BlueprintCallable)
	UItemContainer* GetInventoryContainer() { return Inventory; }
	UFUNCTION(BlueprintCallable)
	UItemContainer* GetQuickAccessContainer() { return QuickAccessBar; }
	// Update all UI elements
	UFUNCTION(BlueprintCallable)
	void UpdateWidgets();
};
