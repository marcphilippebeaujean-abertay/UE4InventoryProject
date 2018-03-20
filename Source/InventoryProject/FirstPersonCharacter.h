// Copyright © 2018, Marc Philippe Beaujean

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Blueprint/UserWidget.h"
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

	// Variable that holds the widget after it is assigned in the blue print
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> DefaultInterfaceWidgetClass;

	// How far the ray reaches ahead of the player
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Grab", Meta = (BlueprintProtected = "true"))
	float GrabDistance = 90.0f;

	// The rotation of an object after it has been grabbed
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Grab", Meta = (BlueprintProtected = "true"))
	FRotator DefaultGrabRotation;

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

	// Grab object in reach of raycast
	void GrabObject();

	// Drop the physics object being held by the player
	void ReleaseObject();

	// Finds all attached components
	void FindActorComponents();

	// Raycast dependancies
	const FVector GetRayEndPoint();
	FVector PlayerViewPointLocation;

	// Return hit from raycast
	const FHitResult GetTraceResult();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

};