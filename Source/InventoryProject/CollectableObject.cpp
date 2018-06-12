// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectableObject.h"
#include "Components/StaticMeshComponent.h"
#include "ItemContainer.h"

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
	// Initialise item values
	if (MaxUnitsPerSlot <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Max items per slot noted assigned to object!"));
		MaxUnitsPerSlot = 1;
	}
	CurUnitsInSlot = FMath::Clamp(InitUnits, 1, MaxUnitsPerSlot);
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
	// Setup collision preset for mesh to be compatible with the pickup trace
	Mesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	// Enable physics
	Mesh->SetSimulatePhysics(true);
}

void ACollectableObject::OnObjectCollected(UItemContainer* NewOwner)
{
	// Deactivate collision for the actor
	this->SetActorEnableCollision(false);
	// Hide actor from being visible while disabled
	this->SetActorHiddenInGame(true);
	// Stop actor from ticking
	this->SetActorTickEnabled(false);
	// Disable physics
	Mesh->SetSimulatePhysics(false);
	// Setup reference to the player and inventory
	UpdateObjectOwner(NewOwner);
}

void ACollectableObject::DropItem(FVector DropLocation)
{
	// Reposition the collectable to the position where it is being dropped (Player's position? Near a lootbox?)
	this->SetActorLocation(DropLocation);
	// Activate collision for the actor
	this->SetActorEnableCollision(true);
	// Make actor visible
	this->SetActorHiddenInGame(false);
	// Make actor visible
	this->SetActorHiddenInGame(false);
	// Disable physics
	Mesh->SetSimulatePhysics(true);
	// Reset physics velocity
	Mesh->SetPhysicsLinearVelocity(FVector(0, 0, 0));
	// Set object owner to nullptr
	UpdateObjectOwner(nullptr);
}

FString ACollectableObject::GetIndicatorName()
{
	// Check if indicator name needs to include number of items in the slot
	FString ItemNrIndicator = "";
	if (MaxUnitsPerSlot > 1)
	{
		ItemNrIndicator = (" (" + FString::FromInt(CurUnitsInSlot) + ")");
	}
	return (IndicatorDisplayName + ItemNrIndicator);
}

void ACollectableObject::UpdateObjectOwner(UItemContainer* NewOwner)
{
	// Setup reference to the new container
	OwningContainer = NewOwner;
	if (OwningContainer == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to assign inventory!"));
	}
	// Check if container belongs to a player, if so, assign the new player as player
	if(AFirstPersonCharacter* NewPlayerOwner = Cast<AFirstPersonCharacter>(OwningContainer->GetOwner()))
	{
		OwningPlayer = NewPlayerOwner;
	}
	else
	{
		OwningPlayer = nullptr;
		UE_LOG(LogTemp, Error, TEXT("Failed to cast container own to player!"));
	}
	// Check if we need to combine items in the inventory, since they have a similiar resource
	if(ItemResourceType != EResourceType::None)
	{
		CheckForCommonResource();
	}
}

void ACollectableObject::CheckForCommonResource()
{
	if (this->CurUnitsInSlot != MaxUnitsPerSlot)
	{
		for (int i = 0; i < OwningContainer->GetContainerItems().Num(); i++)
		{
			ACollectableObject* NewContainerItem = OwningContainer->GetContainerItem(i);
			// Check if the items resource type is identical to the one 
			if (NewContainerItem->GetItemResourceType() == ItemResourceType)
			{
				// Check if that item does not have the max number of slots filled
				if (NewContainerItem->GetCurUnitsInSlot() < MaxUnitsPerSlot)
				{
					// Get maximum number that can be added to the new item slot from the current one
					int ItemsToBeAdded = FMath::Clamp(CurUnitsInSlot, 0, MaxUnitsPerSlot - NewContainerItem->GetCurUnitsInSlot());
					// Add to the other container item
					NewContainerItem->IncrementItemCount(ItemsToBeAdded);
					// Subtract from curent container item
					DecrementItemCount(ItemsToBeAdded);
					// Check if local items have not been depleted
					if (CurUnitsInSlot <= 0)
					{
						break;
					}
				}
			}
		}
	}
}
