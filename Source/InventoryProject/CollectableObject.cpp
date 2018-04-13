// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectableObject.h"
#include "Components/StaticMeshComponent.h"
#include "PlayerInventory.h"

// Sets default values
ACollectableObject::ACollectableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Apply editor-specified variables to the local temps created in the script
	AssignDefaultComponents();
}

// Called when the game starts or when spawned
void ACollectableObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACollectableObject::AssignDefaultComponents()
{
	// Create mesh object
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	// Make mesh object the root (since it contains collision, physics, etc.)
	RootComponent = Mesh;
	// Attach to root
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ACollectableObject::CollectObject(AFirstPersonCharacter* NewOwner)
{
	// Deactivate collision for the actor
	this->SetActorEnableCollision(false);
	// Hide actor from being visible while disabled
	this->SetActorHiddenInGame(true);
	// Stop actor from ticking
	this->SetActorTickEnabled(false);
	// Set the new owner of the object
	ObjectOwner = NewOwner;
	if (ObjectOwner == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to assign owner to object!"));
	}
	// Get reference to the inventory
	PlayerInventoryRef = NewOwner->FindComponentByClass<UPlayerInventory>();
	if (PlayerInventoryRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to assign inventory!"));
	}
}

void ACollectableObject::DropItem(FVector DropLocation)
{
	// Reposition the collectable to the position where it is being dropped (Player's position? Near a lootbox?)
	this->SetActorLocation(DropLocation);
	// Activate collision for the actor
	this->SetActorEnableCollision(true);
	// Make actor visible
	this->SetActorHiddenInGame(false);
	// Actor should tick
	this->SetActorTickEnabled(true);
}