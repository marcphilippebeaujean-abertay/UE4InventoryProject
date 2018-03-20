// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	// The name of an object as it should be displayed when the player hovers over it
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (BlueprintProtected = "true"))
	FString IndicatorDisplayName;

	// Mesh that should be displayed in the scene
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (BlueprintProtected = "true"))
	UStaticMesh* ObjectMesh = nullptr;

	// 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Function that allows the character to access the object name
	UFUNCTION()
	FString GetIndicatorName() { return IndicatorDisplayName; }
	
};
