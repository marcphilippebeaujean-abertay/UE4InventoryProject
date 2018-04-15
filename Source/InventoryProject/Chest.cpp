// Fill out your copyright notice in the Description page of Project Settings.

#include "Chest.h"


// Sets default values
AChest::AChest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh object
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	// Make mesh object the root (since it contains collision, physics, etc.)
	RootComponent = Mesh;
	// Attach to root
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	// Setup collision preset for mesh to be compatible with the pickup trace
	Mesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);

	// Initialise the item container
	ItemContainer = CreateDefaultSubobject<UItemContainer>(TEXT("Item Container"));
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	

}
