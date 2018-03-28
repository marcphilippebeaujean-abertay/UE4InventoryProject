// Fill out your copyright notice in the Description page of Project Settings.

#include "NewPlayerController.h"

void ANewPlayerController::BeginPlay()
{
	// Create interface widgets when the game starts
	InitInterfaceWidgets();
	// Initialise the containers
	InitContainers();
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



}
