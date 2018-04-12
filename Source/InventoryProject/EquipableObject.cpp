// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipableObject.h"


void AEquipableObject::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	// Set equipable object to true
	bEquipable = true;
}


void AEquipableObject::EquipItem()
{
	if (ObjectOwner)
	{
		// Set mesh of item for the player
		ObjectOwner->SetItemMesh(Mesh->GetStaticMesh());
		bEquiped = true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find object owner!"));
	}
}

void AEquipableObject::UnEquipItem()
{
	if (ObjectOwner)
	{
		// Hide item mesh if its being unequiped
		ObjectOwner->HideItemMesh();
		bEquiped = false;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find object owner!"));
	}
}
