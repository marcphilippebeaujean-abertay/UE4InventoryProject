// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

// Sets default values
AInteractable::AInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Find components needed to implement interactable
	AssignDefaultComponents();

}

// Called when the game starts or when spawned
void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FString AInteractable::GetIndicatorName()
{
	return IndicatorDisplayName;
}

void AInteractable::AssignDefaultComponents()
{
	// Create mesh object
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	// Make mesh object the root (since it contains collision, physics, etc.)
	RootComponent = Mesh;
	// Attach to root
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	// Setup collision preset for mesh to be compatible with the pickup trace
	Mesh->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
	// Set constraint mode
	Mesh->SetConstraintMode(EDOFMode::SixDOF);
	// Enable physics
	Mesh->SetSimulatePhysics(true);
}

bool AInteractable::CanInteract()
{
	return true;
}
