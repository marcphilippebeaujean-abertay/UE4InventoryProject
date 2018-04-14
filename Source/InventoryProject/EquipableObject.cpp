// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipableObject.h"
#include "ItemContainer.h"

void AEquipableObject::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	// Set equipable object to true
	bEquipable = true;
}


void AEquipableObject::EquipItem()
{
	if (OwningPlayer)
	{
		// Set mesh of item for the player
		OwningPlayer->SetItemMesh(Mesh->GetStaticMesh(), Mesh->GetMaterial(0));
		bEquiped = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find object owner!"));
	}
}

void AEquipableObject::UnEquipItem()
{
	if (OwningPlayer)
	{
		// Hide item mesh if its being unequiped
		OwningPlayer->HideItemMesh();
		bEquiped = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find object owner!"));
	}
}

void AEquipableObject::UseItem()
{
	
}