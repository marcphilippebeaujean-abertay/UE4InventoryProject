// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Interactable.generated.h"

UCLASS()
class INVENTORYPROJECT_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	// Function that allows the character to access the object name
	UFUNCTION(BlueprintCallable)
	FString GetIndicatorName();

	// Function calls when player wishes to interact with the object
	virtual void OnInteracted() PURE_VIRTUAL(AInteractable::OnInteracted, );

	// Boolean that determines if the object can be interacted with
	virtual bool CanInteract();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The name of an object as it should be displayed when the player is in range to interact with it
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	FString IndicatorDisplayName;

	// Variable that allows the designer to assign a mesh in the editor
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh = nullptr;

	virtual void AssignDefaultComponents();
	
};
