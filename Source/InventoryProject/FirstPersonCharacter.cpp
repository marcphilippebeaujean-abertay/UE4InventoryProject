// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonCharacter.h"


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
	// Setup interface after the game has started
	SetupInterface();
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
		PhysicsHandle->SetTargetLocationAndRotation(GetRayEndPoint(), GetActorRotation());
	}
	// Check if we can detect a grabable object
	if (ACollectableObject* hitCollectable = Cast<ACollectableObject>(GetTraceResult().GetActor()))
	{
		UE_LOG(LogTemp, Error, TEXT("Traced a collectable object!"));
		GrabIndicator = "Click to pick up " + hitCollectable->GetIndicatorName();
	}
	else
	{
		// Otherwise, we have not hit an actor - don't display indicator
		GrabIndicator = "";
	}
}

// Called to bind functionality to input
void AFirstPersonCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup movement input
	InputComponent->BindAxis("Forward", this, &AFirstPersonCharacter::MoveForward);
	InputComponent->BindAxis("Right", this, &AFirstPersonCharacter::MoveRight);
	// Setup mouse input
	InputComponent->BindAxis("Turn", this, &AFirstPersonCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AFirstPersonCharacter::AddControllerPitchInput);
	// Bind input actions related to grabbing objects
	InputComponent->BindAction("Grab", IE_Pressed, this, &AFirstPersonCharacter::GrabObject);
	InputComponent->BindAction("Grab", IE_Released, this, &AFirstPersonCharacter::ReleaseObject);
}

void AFirstPersonCharacter::SetupInterface()
{
	// Check the selected UI class is not NULL
	if (DefaultInterfaceWidgetClass)
	{
		// If the widget is not created and == NULL
		if (!DefaultInterfaceWidget)
		{
			// Check if player is being possesed by a controller
			if (PlCtrler)
			{
				UE_LOG(LogTemp, Warning, TEXT("Creating interface widget!"));
				// Create Widget by accessing the player controller
				DefaultInterfaceWidget = CreateWidget<UUserWidget>(PlCtrler, DefaultInterfaceWidgetClass);
			}
			if (!DefaultInterfaceWidget)
			{
				// Something went wrong!
				return;
			}
			// Add it to the viewport so the Construct() method in the UUserWidget:: is run
			DefaultInterfaceWidget->AddToViewport();
		}
	}
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
	// Get player controller from the world - since it is a one player game, ID will be 0
	PlCtrler = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
}

void AFirstPersonCharacter::GrabObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Attempting to grab object!"));
	FHitResult hit = GetTraceResult();
	// Deduce the actor that was hit by the trace
	auto ActorHit = hit.GetActor();
	auto PhysicsComponent = hit.GetComponent();
	// Check if actor was detected
	if (ActorHit)
	{
		// Check if a physics component has been detected
		if (PhysicsComponent)
		{
			// Grab component via the physics handle
			PhysicsHandle->GrabComponentAtLocationWithRotation(PhysicsComponent, NAME_None, ActorHit->GetActorLocation(), DefaultGrabRotation);
		}
	}
}

void AFirstPersonCharacter::ReleaseObject()
{
	UE_LOG(LogTemp, Warning, TEXT("Releasing object!"));
	// Release component
	PhysicsHandle->ReleaseComponent();
}

const FVector AFirstPersonCharacter::GetRayEndPoint()
{
	// Get player view point on tick
	FRotator PlayerViewPointRotation;
	// Strange syntax - the parameters past into the function are manipulated (returned?)
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	// Create the ray end point
	FVector RayEndPoint = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * GrabDistance;

	return RayEndPoint;
}


FHitResult AFirstPersonCharacter::GetTraceResult()
{
	// Define ray collision specifications
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, this);
	// Create the ray
	FHitResult Hit;
	// Update view related variables - "PlayerViewPointLocation" is updated here, hence why we need to call it first
	FVector RayEndPoint = GetRayEndPoint();
	// Function that sets "hit" to contain the information of our ray cast
	if (GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewPointLocation, RayEndPoint, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters))
	{
		// See if we hit a physics object
		if (AActor* ActorHit = Hit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Name of hit Object is: %s"), *ActorHit->GetName());
		}
	}
	return Hit;
}

void AFirstPersonCharacter::MoveForward(float val)
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

void AFirstPersonCharacter::MoveRight(float val)
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
