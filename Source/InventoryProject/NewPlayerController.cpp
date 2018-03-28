// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPlayerController.h"

ANewPlayerController::ANewPlayerController()
{

}


void ANewPlayerController::BeginPlay()
{
	// Set actor to tick
	PrimaryActorTick.bCanEverTick = true;
	// Initialise reference to player character
	if (ACharacter* PlChar = GetCharacter())
	{
		PlayerCharacter = Cast<AFirstPersonCharacter>(PlChar);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find character!"));
		return;
	}
	// Initialise the containers
	InitContainers();
	// Create interface widgets when the game starts
	InitInterfaceWidgets();
}

void ANewPlayerController::Tick(float DeltaSeconds)
{
	// Update the characters view point for line tracing
	UpdateCharacterViewpoint();
	// Check if we can detect a grabable object
	if (ACollectableObject* HitCollectable = Cast<ACollectableObject>(PlayerCharacter->GetTraceResult().GetActor()))
	{
		// Make sure we aren't holding another component
		if (!PlayerCharacter->CarryingPhysicsObject())
		{
			// Show indicator that tells the player what they can pick up
			InteractionIndicator = "Click to pick up " + HitCollectable->GetIndicatorName();
		}
		else
		{
			// Don't display indicator
			InteractionIndicator = "";
		}
	}
	else
	{
		// Otherwise, we have not hit an actor - don't display indicator
		InteractionIndicator = "";
	}
}

void ANewPlayerController::InitInterfaceWidgets()
{
	// Check the selected UI class is not NULL
	if (DefaultInterfaceWidgetClass)
	{
		// Create Widget by accessing the player controller
		DefaultInterfaceWidget = CreateWidget<UUserWidget>(this, DefaultInterfaceWidgetClass);
		if (!DefaultInterfaceWidget)
		{
			// Something went wrong!
			return;
		}
		// Add it to the viewport so the Construct() method in the UUserWidget:: is run
		DefaultInterfaceWidget->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find interface widget template!"));
		return;
	}
	// Set inventory widget
	InventoryWidget = DefaultInterfaceWidget->GetWidgetFromName("Inventory_Container");
	InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	// Broadcast a widget update
	UpdateWidgets();
}

void ANewPlayerController::InitContainers()
{
	// Find player inventory
	Inventory = this->FindComponentByClass<UPlayerInventory>();
	if(Inventory == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find inventory!"));
	}
	// Find quick access component
	QuickAccessBar = this->FindComponentByClass<UQuickAccess>();
	if(QuickAccessBar == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find quick access bar!"));
	}
}

void ANewPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Handle mouse look
	InputComponent->BindAxis("Turn", this, &ANewPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &ANewPlayerController::AddPitchInput);
	// Grab object
	InputComponent->BindAction("Grab", IE_Pressed, this, &ANewPlayerController::GrabObject);
	InputComponent->BindAction("Grab", IE_Released, this, &ANewPlayerController::OnGrabRelease);
	// Toggle inventory
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &ANewPlayerController::ToggleInventory);
}

void ANewPlayerController::GrabObject()
{
	// Get the hit result to see if we grabbed a collectable object
	FHitResult TraceHit = PlayerCharacter->GetTraceResult();
	// Deduce the actor that was hit by the trace
	auto ActorHit = TraceHit.GetActor();
	// Check if we got a hit
	if(ActorHit)
	{
		// Check if the other actor is of type collectable
		if(ACollectableObject* HitCollectable = Cast<ACollectableObject>(ActorHit))
		{
			// Use the inventory to collect the item
			Inventory->CollectObject(HitCollectable);
			return;
		}
		// Attempt to grab a physics object if the hit object is not of type collectable
		PlayerCharacter->GrabPhysicsObject();
	}
}

void ANewPlayerController::OnGrabRelease()
{
	PlayerCharacter->ReleasePhysicsObject();
}


void ANewPlayerController::UpdateWidgets()
{
	Inventory->BroadcastWidgetUpdate();
	QuickAccessBar->BroadcastWidgetUpdate();
}

void ANewPlayerController::ToggleInventory()
{
	// Toggle the inventory bool
	bInventoryOpen = !bInventoryOpen;
	// Enable/disable mouse cursor to navigate the inventory
	this->bShowMouseCursor = bInventoryOpen;
	// Enable/disable UI navigation
	this->bEnableClickEvents = bInventoryOpen;
	this->bEnableMouseOverEvents = bInventoryOpen;
	// Enable/disable player camera rotation
	this->SetIgnoreLookInput(bInventoryOpen);
	if (InventoryWidget)
	{
		// Determine if the inventory should be visible or not
		if (bInventoryOpen)
		{
			// Make sure to update the inventory before making it visible
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Widget missing!"));
	}
}

void ANewPlayerController::UpdateCharacterViewpoint()
{
	// Retrieve view point location + rotation
	FVector CurViewPointLocation;
	FRotator CurViewPointRotation;
	GetPlayerViewPoint(CurViewPointLocation, CurViewPointRotation);
	// Set the varaibles in the character
	PlayerCharacter->SetPlayerViewLocation(CurViewPointLocation);
	PlayerCharacter->SetPlayerViewRotation(CurViewPointRotation);
}