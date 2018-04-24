// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"
#include "QuickAccess.h"
#include "PlayerInventory.h"
#include "LanternItemComponent.h"

// Sets default values
AFirstPersonCharacter::AFirstPersonCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Find actor components (and other external dependancies)
	InitActorComponents();
}

// Called when the game starts or when spawned
void AFirstPersonCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFirstPersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find physics handle!"));
		return;
	}
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move object we are holding
		PhysicsHandle->SetTargetLocationAndRotation(GetRayEndPoint(false), GetActorRotation());
	}
	// Get the hit result to see if we grabbed a collectable object
	FHitResult TraceHit = GetTraceResult();
	// Deduce the actor that was hit by the trace
	auto ActorHit = TraceHit.GetActor();
	// Check if we got a hit
	if (ActorHit)
	{
		// Check if the other actor is of type collectable
		if (ACollectableObject* HitCollectable = Cast<ACollectableObject>(ActorHit))
		{
			// Update indicator using the recieved object
			OnUpdateIndicator.Broadcast(HitCollectable->GetIndicatorName());
			// Set indicator to require reset
			bIndicatorReset = false;
			return;
		}
	}
	else
	{
		// Check if indicator needs to be updated
		if (!bIndicatorReset)
		{
			bIndicatorReset = true;
			OnUpdateIndicator.Broadcast("");
		}
	}
}

void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup movement input
	InputComponent->BindAxis("Forward", this, &AFirstPersonCharacter::MoveForward);
	InputComponent->BindAxis("Right", this, &AFirstPersonCharacter::MoveRight);
}

void AFirstPersonCharacter::InitActorComponents()
{
	// Create new physics handle
	this->PhysicsHandle = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));
	// Check if all components were found successfully
	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to generate physics handle!"));
		return;
	}

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	ItemMesh->SetOnlyOwnerSee(true);
	ItemMesh->SetupAttachment(FirstPersonCameraComponent);
	ItemMesh->bCastDynamicShadow = false;
	ItemMesh->CastShadow = false;

	// Initialise the light component
	LanternLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("LanternLight"));
	LanternLight->SetupAttachment(ItemMesh);
	LanternLight->SetAttenuationRadius(0.0f);
	LanternLight->SetIntensity(10.0f);
	LanternLight->SetSourceRadius(0.0f);

	// Create inventory related components
	Inventory = CreateDefaultSubobject<UPlayerInventory>(TEXT("PlayerInventory"));
	QuickAccess = CreateDefaultSubobject<UQuickAccess>(TEXT("QuickAccess"));

	// Create item components
	LanternItem = CreateDefaultSubobject<ULanternItemComponent>(TEXT("LanternComponent"));
	
}

const FVector AFirstPersonCharacter::GetRayEndPoint(bool IsGrabbing)
{
	// Strange syntax - the parameters past into the function are manipulated (returned?)
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	// Temporary variable that stores the grab distance
	float FinalGrabDistance = GrabDistance;
	// Check if the player is looking at the floor beneath them
	if (LookingAtFloor() && IsGrabbing)
	{
		// To simulate the player being able to crouch down and grab objects beneath them, the grab distance is multiplied
		FinalGrabDistance *= 2;
	}
	// Create the ray end point
	FVector RayEndPoint = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * FinalGrabDistance;

	return RayEndPoint;
}

FHitResult AFirstPersonCharacter::GetTraceResult()
{
	// Define ray collision specifications
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, this);
	// Create the ray
	FHitResult Hit;
	// Update view related variables - "PlayerViewPointLocation" is updated here, hence why we need to call it first
	FVector RayEndPoint = GetRayEndPoint(true);
	// Function that sets "Hit" to contain the information of our ray cast
	if (GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewPointLocation, RayEndPoint, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters))
	{
		// Sent out trace
	}
	return Hit;
}

void AFirstPersonCharacter::MoveForward(float val)
{
	// Check if player should move
	if (bShouldMove)
	{
		if ((Controller != NULL) && (val != 0.0f))
		{
			// find out which way is forward
			FRotator Rotation = Controller->GetControlRotation();
			// Limit pitch when walking or falling
			if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
			{
				Rotation.Pitch = 0.0f;
			}
			// add movement in that direction
			const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
			AddMovementInput(Direction, val);
		}
	}
}

void AFirstPersonCharacter::MoveRight(float val)
{
	// Check if player should move
	if (bShouldMove)
	{
		if ((Controller != NULL) && (val != 0.0f))
		{
			// find out which way is right
			const FRotator Rotation = Controller->GetControlRotation();
			const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
			// add movement in that direction
			AddMovementInput(Direction, val);
		}
	}
}

bool AFirstPersonCharacter::LookingAtFloor()
{
	// Check the view point pitch rotation and compare to angles to determine if player is looking down
	if (PlayerViewPointRotation.Pitch > 270 && PlayerViewPointRotation.Pitch < 290)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AFirstPersonCharacter::GrabPhysicsObject(UPrimitiveComponent* PhysicsComponent, FVector HitActorLocation)
{
	// Otherwise, check if it's a physics object
	if (PhysicsComponent)
	{
		// Grab component via the physics handle
		PhysicsHandle->GrabComponentAtLocationWithRotation(PhysicsComponent, NAME_None, HitActorLocation, DefaultGrabRotation);
	}
}

void AFirstPersonCharacter::ReleasePhysicsObject()
{
	// Release component
	PhysicsHandle->ReleaseComponent();
}

bool AFirstPersonCharacter::CarryingPhysicsObject()
{
	if(PhysicsHandle->GrabbedComponent)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void AFirstPersonCharacter::SetItemMesh(UStaticMesh* NewItemMesh, UMaterialInterface* NewItemMaterial)
{
	// Set visibility to true
	ItemMesh->SetHiddenInGame(false);
	// Set the new mesh
	ItemMesh->SetStaticMesh(NewItemMesh);
	// Set the new material
	ItemMesh->SetMaterial(0, NewItemMaterial);

}

void AFirstPersonCharacter::HideItemMesh()
{
	ItemMesh->SetHiddenInGame(true);
}