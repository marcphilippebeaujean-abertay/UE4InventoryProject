// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "FirstPersonCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "CollectableObject.generated.h"

UCLASS()
class INVENTORYPROJECT_API ACollectableObject : public AActor
{
	GENERATED_BODY()
	
public:	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function that applies variables assigned in the editor
	void AssignDefaultComponents();

	// Sets default values for this actor's properties
	ACollectableObject();

	// The name of an object as it should be displayed when the player hovers over it and in the inventory UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	FString IndicatorDisplayName;

	// Condition that checks if an object can be moved to quick access
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Inventory Specifications", Meta = (AllowPrivateAccess = "true"))
	bool bEquipable = false;

	// Specifies if the item can be dragged in the inventory
	bool bEmptySlot = false;

	// The character that owns the object
	AFirstPersonCharacter* ObjectOwner = nullptr;

	// Variable that allows the designer to assign a mesh in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that allows the character to access the object name
	UFUNCTION()
	virtual FString GetIndicatorName() { return IndicatorDisplayName; }

	// Returns the object to the scene as a collectable
	void DropItem(FVector DropLocation);

	// Handle behaviour when object is collected by the player (disable mesh and collision components etc.)
	virtual void CollectObject(AFirstPersonCharacter* NewOwner);
	
	// Used to make distinctions between empty slots and normal items in the blueprints script
	UFUNCTION(BlueprintCallable)
	bool IsEmptySlot() { return bEmptySlot; }

	UFUNCTION(BlueprintCallable)
	bool IsEquipable() { return bEquipable; }

private:

	// Icon that should be displayed to represent the item in the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	UTexture2D* Thumbnail = nullptr;
};
