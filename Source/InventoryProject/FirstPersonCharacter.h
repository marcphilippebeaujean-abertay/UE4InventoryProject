// Copyright © 2018, Marc Philippe Beaujean

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CollectableObject.h"
#include "ItemContainer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Array.h"
#include "FirstPersonCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryInterfaceUpdate, const TArray<ACollectableObject*>&, InventoryContents);

UCLASS()
class INVENTORYPROJECT_API AFirstPersonCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AFirstPersonCharacter();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Basic movement functions bound to the axis
	void MoveForward(float val);
	void MoveRight(float val);

private:

	// Interface used when player does not have the inventory open
	UPROPERTY()
	UUserWidget* DefaultInterfaceWidget = nullptr;

	// Reference to the player controller
	UPROPERTY()
	APlayerController* PlCtrler = nullptr;

	// Physics handle component that we will access
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Inventory containing all the objects the player collected
	UPROPERTY()
	UItemContainer* Inventory = nullptr;

	// Grab object in reach of raycast
	void GrabObject();

	// Drop the physics object being held by the player
	void ReleasePhysicsObject();

	// Create/find dependancies
	void InitActorComponents();

	// Creates the initial interface when the game starts
	void SetupInterface();

	// Raycast dependancies
	const FVector GetRayEndPoint();
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	// Return hit from raycast
	FHitResult GetTraceResult();

	// Update the string that indicates if an object is in range of being picked up
	FString GrabIndicator;

	// Variable that holds the widget after it is assigned in the blue print
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> DefaultInterfaceWidgetClass;

	// Item that is used for empty slots in the inventory - makes it easier to switch between item slots and allows for more customisation by the designer
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACollectableObject> EmptySlotClass;
	ACollectableObject* EmptySlot = nullptr;

	// How far the ray reaches ahead of the player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	float GrabDistance = 90.0f;

	// The rotation of an object after it has been grabbed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	FRotator DefaultGrabRotation;

	// Distance away from that the player, that dropped items are reinstantiated
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory", Meta = (AllowPrivateAccess = "true"))
	int DropDistance = 10;

	// Check to see if camera is angled at the ground
	bool LookingAtFloor();

	// Function that toggles the inventory, as well as the player's ability to see the cursor
	void ToggleInventory();
	bool bInventoryOpen = false;

	// Displays the bulk of the inventory
	UUserWidget* InventoryWidget = nullptr;

	// Initialises inventory component and broadcasts an update to the widget
	void InitInventory();

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function that allows the UI widget to access the string
	UFUNCTION(BlueprintPure)
	FString GetGrabIndicator() { return GrabIndicator; }

	// Updates the inventory by casting our inventory array to the widget blueprint
	UFUNCTION(BlueprintCallable)
	void UpdateInventoryWidget();

	// Updates the quick access UI widget
	UFUNCTION(BlueprintCallable)
	void UpdateQuickAccessWidget();

	// Delegate used to broadcast to the blueprint that theinvneotyr has been updated
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FInventoryInterfaceUpdate OnUpdateInventory;

	// Delegate used to broadcast to the blueprint
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FInventoryInterfaceUpdate OnUpdateQuickAccess;

private:

};