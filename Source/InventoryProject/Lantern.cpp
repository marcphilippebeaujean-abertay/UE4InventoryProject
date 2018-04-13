// Fill out your copyright notice in the Description page of Project Settings.

#include "Lantern.h"


void ALantern::EquipItem()
{
	Super::EquipItem();
	if (LanternComponent)
	{
		// Enable tick on the lantern component
		LanternComponent->SetComponentTickEnabled(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find lantern component!"));
		return;
	}
}

void ALantern::UnEquipItem()
{
	
}


void ALantern::CollectObject(AFirstPersonCharacter* NewOwner)
{
	Super::CollectObject(NewOwner);
	// Check that owner has been assigned correctly
	if (ObjectOwner)
	{
		// Locate and assign lantern item component, which is responsible for handling the light cast by the lantern
		LanternComponent = ObjectOwner->FindComponentByClass<ULanternItemComponent>();
		if(LanternComponent)
		{
			
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to find lantern component!"));
			return;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find owning player!"));
		return;
	}
}

void ALantern::UseItem()
{
	Super::UseItem();
	UE_LOG(LogTemp, Error, TEXT("Trying to use item!"));
	if (LanternComponent)
	{
		// Relight the lantern component
		LanternComponent->RelightLantern();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find lantern component!"));
		return;
	}
}
