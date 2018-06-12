// Fill out your copyright notice in the Description page of Project Settings.

#include "Lantern.h"

#include "PlayerInventory.h"

void ALantern::EquipItem()
{
	Super::EquipItem();
	if (!LanternComponent)
	{
		// Locate and assign lantern item component, which is responsible for handling the light cast by the lantern
		LanternComponent = OwningPlayer->FindComponentByClass<ULanternItemComponent>();
	}
	// Enable tick on the lantern component
	LanternComponent->SetComponentTickEnabled(true);
}

void ALantern::UnEquipItem()
{
	Super::UnEquipItem();

	// Extinguish lantern light
	LanternComponent->ExtinguishLantern();
	LanternComponent->SetComponentTickEnabled(false);
}

void ALantern::OnObjectCollected(UItemContainer* NewOwner)
{
	Super::OnObjectCollected(NewOwner);
	// Find lantern component
	if(OwningPlayer)
	{
		LanternComponent = OwningPlayer->FindComponentByClass<ULanternItemComponent>();
	}
}

void ALantern::UseItem()
{
	// Create player inventory reference
	UPlayerInventory* PlayerInventory = OwningPlayer->FindComponentByClass<UPlayerInventory>();
	// Find matches resource in the inventory
	ACollectableObject* MatchStack = PlayerInventory->GetResourceOfType(EResourceType::Matches);
	// Check if a match stack is available
	if (MatchStack)
	{
		if (LanternComponent)
		{
			// Relight the lantern component
			LanternComponent->RelightLantern();
			// Subtract item count from match stack
			MatchStack->DecrementItemCount(1);
			if(MatchStack->GetCurUnitsInSlot() <= 0)
			{
				// If match stack is depleted, remove from inventory
				PlayerInventory->CheckForDepletedItems();
				PlayerInventory->BroadcastWidgetUpdate();
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find lantern component!"));
		}
	}
}
