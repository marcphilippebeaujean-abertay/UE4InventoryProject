// Fill out your copyright notice in the Description page of Project Settings.

#include "Lantern.h"
#include "ItemContainer.h"

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
	if (LanternComponent)
	{
		// Relight the lantern component
		LanternComponent->RelightLantern();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find lantern component!"));
	}
}
