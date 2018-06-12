// Fill out your copyright notice in the Description page of Project Settings.

#include "FirstPersonPlayerController.h"
#include "Interactable.h"
#include "MyGameInstance.h"
#include "MySaveGame.h"

AFirstPersonPlayerController::AFirstPersonPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

}


void AFirstPersonPlayerController::BeginPlay()
{
	PlayerCharacter = Cast<AFirstPersonCharacter>(GetCharacter());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find character!"));
	}
	// Find and create the containers
	InitContainers();
	// Create the UI widgets
	InitInterfaceWidgets();
	// Find player character
}

void AFirstPersonPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}


void AFirstPersonPlayerController::InitInterfaceWidgets()
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
	// Set external container widget
	ExternalContainerWidget = DefaultInterfaceWidget->GetWidgetFromName("External_Container");
	ExternalContainerWidget->SetVisibility(ESlateVisibility::Hidden);
	// Broadcast a widget update
	UpdateWidgets();
}

void AFirstPersonPlayerController::InitContainers()
{
	// Spawn empty slot actor
	EmptySlot = Cast<ACollectableObject>(this->GetWorld()->SpawnActor<ADefaultEmptySlot>());
	// Set owner of slot to be the character
	EmptySlot->SetObjectOwner(PlayerCharacter);
	// Check for correct game instance
	if(!Cast<UMyGameInstance>(GetGameInstance()))
	{
		UE_LOG(LogTemp, Error, TEXT("Wrong game instance assigned in project settings - please update!"));
		return;
	}
	// Check if the containers exist...
	if (Inventory == nullptr || QuickAccessBar == nullptr)
	{
		// ...initialise containers for the first time
		if (EmptySlot)
		{
			// Find player inventory
			Inventory = PlayerCharacter->FindComponentByClass<UPlayerInventory>();
			Inventory->InitContainerContents(EmptySlot);
			if (Inventory == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find inventory!"));
			}
			// Find quick access component
			QuickAccessBar = PlayerCharacter->FindComponentByClass<UQuickAccess>();
			if (QuickAccessBar == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to find quick access bar!"));
			}
			QuickAccessBar->InitContainerContents(EmptySlot);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find empty slot class!"));
			return;
		}

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Containers initialised prematurely!"));
		return;
	}
	// Check if prior information on the object has been stored
	if (Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))->SaveDataAvailable() == true)
	{
		// Retrieve reference to current save object
		UMySaveGame* LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		LoadGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->GetSaveSlotName(), LoadGameInstance->GetUserIndex()));
		// Load containers from save object 
		//Inventory->SetContainerItems(LoadGameInstance->GetPlayerInventory()->GetContainerItems());
		//
		//QuickAccessBar->SetContainerItems(LoadGameInstance->GetPlayerQuickAccess()->GetContainerItems());
		//if (Inventory)
		//{
		//	UE_LOG(LogTemp, Warning, TEXT("Retrieved items from game instance...!"));
		//	// ...set containers of the player to be that in the game instance
		//	PlayerCharacter->SetPlayerContainers(Inventory, QuickAccessBar);
		//	// Update the container widgets
		//	Inventory->BroadcastWidgetUpdate();
		//	QuickAccessBar->BroadcastWidgetUpdate();
		//}
	}
}

void AFirstPersonPlayerController::UpdateWidgets()
{
	Inventory->BroadcastWidgetUpdate();
	QuickAccessBar->BroadcastWidgetUpdate();
}

void AFirstPersonPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Handle mouse look
	InputComponent->BindAxis("Turn", this, &AFirstPersonPlayerController::AddYawInput);
	InputComponent->BindAxis("LookUp", this, &AFirstPersonPlayerController::AddPitchInput);
	// Grab object
	InputComponent->BindAction("Grab", IE_Pressed, this, &AFirstPersonPlayerController::InteractWithObject);
	InputComponent->BindAction("Grab", IE_Released, this, &AFirstPersonPlayerController::OnGrabReleased);
	// Toggle inventory
	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AFirstPersonPlayerController::ToggleInventory);
	// Switch between items in the quick access bar
	InputComponent->BindAction("IncrementItemSelection", IE_Pressed, this, &AFirstPersonPlayerController::UpdateItemSelectionInc);
	InputComponent->BindAction("DecrementItemSelection", IE_Pressed, this, &AFirstPersonPlayerController::UpdateItemSelectionDec);
	// Use item selected in the quick access bar
	InputComponent->BindAction("UseInventoryItem", IE_Pressed, this, &AFirstPersonPlayerController::UseCurrentItem);
}

void AFirstPersonPlayerController::ToggleInventory()
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
			// Player should not move if inventory is open
			PlayerCharacter->SetShouldMove(false);
			// Make sure to update the inventory before making it visible
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
			if (ExternalItemContainer != nullptr)
			{
				ExternalContainerWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			// Player should move if inventory is open
			PlayerCharacter->SetShouldMove(true);
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
			if(ExternalItemContainer != nullptr)
			{
				ExternalItemContainer = nullptr;
				ExternalContainerWidget->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory Widget missing!"));
	}
}

void AFirstPersonPlayerController::UpdateItemSelectionInc()
{
	QuickAccessBar->UpdateSelectedItem(true);
}

void AFirstPersonPlayerController::UpdateItemSelectionDec()
{
	QuickAccessBar->UpdateSelectedItem(false);
}

void AFirstPersonPlayerController::InteractWithObject()
{
	if (!bInventoryOpen)
	{
		// Get the hit result to see if we grabbed a collectable object
		FHitResult TraceHit = PlayerCharacter->GetTraceResult();
		// Deduce the actor that was hit by the trace
		auto ActorHit = TraceHit.GetActor();
		// Check if we got a hit
		if (ActorHit)
		{
			// Check if the other actor is of type collectable
			if (ACollectableObject* HitCollectable = Cast<ACollectableObject>(ActorHit))
			{
				// Use the inventory to collect the item
				Inventory->CollectObject(HitCollectable);
				return;
			}
			// Search for external item container in the actor
			if (UItemContainer* OtherContainer = ActorHit->FindComponentByClass<UItemContainer>())
			{
				// Assign external item container
				ExternalItemContainer = OtherContainer;
				// Broadcast the new external container
				OnUpdateExternalContainer.Broadcast(ExternalItemContainer);
				// Check if external container has been initialised
				if (ExternalItemContainer->IsUnitinitialised())
				{
					// Initialise container
					ExternalItemContainer->InitContainerContents(EmptySlot);
				}
				// Broadcast a widget update
				ExternalItemContainer->BroadcastWidgetUpdate();
				// Open the inventory to transfer items between this container
				ToggleInventory();
				return;
			}
			// Check if other actor is of type interactable
			if (AInteractable* HitInteractable = Cast<AInteractable>(ActorHit))
			{
				// Check if player can interact with the object
				if (HitInteractable->CanInteract())
				{
					// Interact with object
					HitInteractable->OnInteracted();
					return;
				}
			}
			// Attempt to grab a physics object if the hit object is not of type collectable
			PlayerCharacter->GrabPhysicsObject(TraceHit.GetComponent(), TraceHit.GetActor()->GetActorLocation());
		}
	}
}

void AFirstPersonPlayerController::OnGrabReleased()
{
	PlayerCharacter->ReleasePhysicsObject();
}

void AFirstPersonPlayerController::UseCurrentItem()
{
	// Check if current selected item is available
	if(QuickAccessBar->GetSelectedItem())
	{
		// Use item that is currently equipped
		QuickAccessBar->GetSelectedItem()->UseItem();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Cur selected item not avilable!"));
		return;
	}
}

void AFirstPersonPlayerController::UpdateGameInstanceInventory()
{
	Cast<UMyGameInstance>(GetGameInstance())->UpdatePlayerContainers(Inventory, QuickAccessBar);
}
