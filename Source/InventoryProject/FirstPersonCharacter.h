// Copyright © 2018, Marc Philippe Beaujean

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "QuickAccess.h"
#include "PlayerInventory.h"
#include "CollectableObject.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Blueprint/UserWidget.h"
#include "Containers/Array.h"
#include "FirstPersonCharacter.generated.h"

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

	// Physics handle component that we will access
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;

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

	// Update the string that indicates if an object is in range of being picked up
	FString GrabIndicator;

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

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Function that allows the UI widget to access the string
	UFUNCTION(BlueprintPure)
	FString GetGrabIndicator() { return GrabIndicator; }

	// Return hit from raycast
	FHitResult GetTraceResult();

};