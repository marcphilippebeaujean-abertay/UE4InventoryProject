// Fill out your copyright notice in the Description page of Project Settings.

#include "CollectableObject.h"
#include "Components/StaticMeshComponent.h"


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
	// Create collider object
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	// Make collider object the root
	RootComponent = SceneComp;

	// Create mesh object
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	// Attach to root
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ACollectableObject::CollectObject()
{
	// Deactivate collision for the actor
	this->SetActorEnableCollision(false);
	// Hide actor from being visible while disabled
	this->SetActorHiddenInGame(true);
	// Stop actor from ticking
	this->SetActorTickEnabled(false);
}