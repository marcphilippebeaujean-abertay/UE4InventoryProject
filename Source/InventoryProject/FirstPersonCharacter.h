// Copyright © 2018, Marc Philippe Beaujean

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/PointLightComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "FirstPersonCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemIndicatorUpdate, const FString, ItemName);

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

private:

	// First person camera
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent = nullptr;

	// Mesh for the item
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh = nullptr;

	// Physics handle component that we will access
	UPROPERTY()
	class UPhysicsHandleComponent* PhysicsHandle = nullptr;

	// Item container components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	class UPlayerInventory* Inventory = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	class UQuickAccess* QuickAccess = nullptr;

	// Item specific components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	class ULanternItemComponent* LanternItem = nullptr;

	// Create/find dependancies
	void InitActorComponents();

	// Variables used to store player view point
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

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

	// Condition thats set to false when for example player is in the inventory and doesnt need to move
	bool bShouldMove = true;

	// Override character movement controls
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	// Basic movement functions bound to the axis
	void MoveForward(float val);
	void MoveRight(float val);

	// Other movement related input
	void StartCrouch();
	void StopCrouch();

	void StartSprint();
	void StopSprint();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Visual, meta = (AllowPrivateAccess = "true"))
	int WalkingSpeed;

	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FItemIndicatorUpdate OnUpdateIndicator;
	bool bIndicatorReset = false;

	UCharacterMovementComponent* MovementComp = nullptr;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Return hit from raycast
	FHitResult GetTraceResult();

	// Grab object in reach of raycast
	void GrabPhysicsObject(UPrimitiveComponent* Component, FVector HitActorLocation);

	// Drop the physics object being held by the player
	void ReleasePhysicsObject();
	// Returns true if playre is carrying physics object
	bool CarryingPhysicsObject();

	// Functions used when a new object is equiped
	void SetItemMesh(UStaticMesh* NewItemMesh, UMaterialInterface* NewItemMaterial);
	void HideItemMesh();

	// Function used by the controller to communicate to the player if it should move
	void SetShouldMove(bool ShouldMove) { bShouldMove = ShouldMove; }

	// Raycast dependancies
	const FVector GetRayEndPoint(bool IsGrabbing);

	// Used to set the containers at the beginning of each level switch
	void SetPlayerContainers(class UPlayerInventory* l_gameInstanceInventory, class UQuickAccess* l_gameInstanceQuickAccess);
};