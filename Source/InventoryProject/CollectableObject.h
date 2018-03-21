// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "CollectableObject.generated.h"

UCLASS()
class INVENTORYPROJECT_API ACollectableObject : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Function that applies variables assigned in the editor
	void AssignDefaultComponents();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that allows the character to access the object name
	UFUNCTION()
	FString GetIndicatorName() { return IndicatorDisplayName; }

	// Handle behaviour when object is collected by the player (disable mesh and collision components etc.)
	virtual void CollectObject();
	
private:

	// The name of an object as it should be displayed when the player hovers over it
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
		FString IndicatorDisplayName;

	// Variable that allows the designer to assign a mesh in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh = nullptr;

	// Collider used for hit detection
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Collision", Meta = (AllowPrivateAccess = "true"))
		USceneComponent* SceneComp = nullptr;

};
