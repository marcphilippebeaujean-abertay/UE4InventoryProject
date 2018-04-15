// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemContainer.h"
#include "Chest.generated.h"

UCLASS()
class INVENTORYPROJECT_API AChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Create item container component
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	UItemContainer* ItemContainer = nullptr;

	// Mesh assigned to the container
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh = nullptr;

public:	

	
	
};
