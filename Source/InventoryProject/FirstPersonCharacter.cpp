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
	// Initialise inventory capacity
	CurCapacity = MaximumInventoryCapaticy;
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
		// Make sure we aren't holding another component
		if (!PhysicsHandle->GrabbedComponent)
		{
			// Show indicator that tells the player what they can pick up
			GrabIndicator = "Click to pick up " + hitCollectable->GetIndicatorName();
		}
		else
		{
			// Don't display indicator
			GrabIndicator = "";
		}
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
	InputComponent->BindAction("Grab", IE_Released, this, &AFirstPersonCharacter::ReleasePhysicsObject);
	// Inventory related input
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AFirstPersonCharacter::ToggleInventory); 
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

const FVector AFirstPersonCharacter::GetRayEndPoint()
{
	// Strange syntax - the parameters past into the function are manipulated (returned?)
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	// Temporary variable that stores the grab distance
	float FinalGrabDistance = GrabDistance;
	// Check if the player is looking at the floor beneath them
	if (LookingAtFloor())
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
	FVector RayEndPoint = GetRayEndPoint();
	// Function that sets "hit" to contain the information of our ray cast
	if (GetWorld()->LineTraceSingleByObjectType(Hit, PlayerViewPointLocation, RayEndPoint, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters))
	{
		// See if we hit a physics object
		if (AActor* ActorHit = Hit.GetActor())
		{

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

void AFirstPersonCharacter::GrabObject()
{
	// Generate hit results
	FHitResult hit = GetTraceResult();
	// Deduce the actor that was hit by the trace
	auto ActorHit = hit.GetActor();
	auto PhysicsComponent = hit.GetComponent();
	// Check if actor was detected
	if (ActorHit)
	{
		// Check if the object is a collectable
		if (ACollectableObject* HitCollectable = Cast<ACollectableObject>(ActorHit))
		{
			// Attempt to collect the object
			HitCollectable->CollectObject();
			// Add the traced object to the inventory
			AddObjToInventory(HitCollectable);
		}
		else
		{
			// Otherwise, check if it's a physics object
			if (PhysicsComponent)
			{
				// Grab component via the physics handle
				PhysicsHandle->GrabComponentAtLocationWithRotation(PhysicsComponent, NAME_None, ActorHit->GetActorLocation(), DefaultGrabRotation);
			}
		}
	}
}

void AFirstPersonCharacter::ReleasePhysicsObject()
{
	// Release component
	PhysicsHandle->ReleaseComponent();
}

void AFirstPersonCharacter::AddObjToInventory(ACollectableObject* NewItem)
{
	// Check if the item has not been destroyed
	if (NewItem != nullptr)
	{
		// Check if the inventory capacity wont exceed the maximum if the item is added
		if (0 < (CurCapacity - NewItem->GetItemWeight()))
		{
			// Subtract the item's weight from the current capacity 
			CurCapacity -= NewItem->GetItemWeight();
			// Add the item to the inventory array
			InventoryContents.Add(NewItem);
			UE_LOG(LogTemp, Error, TEXT("Added %s to inventory, available weight is now %d!"), *NewItem->GetName(), CurCapacity);
			// We added an object to the inventory - let's update the UI
			UpdateInventoryWidget();
		}
	}
}

void AFirstPersonCharacter::DropObjFromInventory()
{
	// Check if inventory is not empty
	if (InventoryContents.Num() > 0)
	{
		// See if object is available
		if (InventoryContents[0] != nullptr)
		{
			// Drop item at player's location
			InventoryContents[0]->DropItem(PlayerViewPointLocation + GetActorForwardVector() * DropDistance);
			UE_LOG(LogTemp, Error, TEXT("Removing %s from inventory!"), *InventoryContents[0]->GetName());
			// Remove corresponding item from the array
			InventoryContents.RemoveAt(0);
			// We added an object to the inventory - let's update the UI
			UpdateInventoryWidget();
		}
	}
	else
	{
		// No items in the inventory - do nothing
		UE_LOG(LogTemp, Error, TEXT("Inventory is empty!"));
	}
}

void AFirstPersonCharacter::UpdateInventoryWidget()
{
	// "Broadcast" our inventory - which creates a reference for the array that is accessible in blueprints
	OnUpdateInventory.Broadcast(InventoryContents);
}

void AFirstPersonCharacter::ToggleInventory()
{
	// Toggle the inventory bool
	bInventoryOpen = !bInventoryOpen;
	if (PlCtrler)
	{
		// Enable/disable mouse cursor to navigate the inventory
		PlCtrler->bShowMouseCursor = bInventoryOpen;
		// Enable/disable UI navigation
		PlCtrler->bEnableClickEvents = bInventoryOpen;
		PlCtrler->bEnableMouseOverEvents = bInventoryOpen;
		// Stop/enable player camera rotation
		PlCtrler->SetIgnoreLookInput(bInventoryOpen);
	}
	// Find inventory widget
	UWidget* InventoryWidget = DefaultInterfaceWidget->GetWidgetFromName(FName("Inventory"));
	if (InventoryWidget)
	{
		// Determine if the inventory should be visible or not
		if (bInventoryOpen)
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

