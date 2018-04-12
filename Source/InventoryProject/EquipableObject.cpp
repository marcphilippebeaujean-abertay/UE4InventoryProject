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
	// Set mesh of item for the player
	ObjectOwner->SetItemMesh(Mesh->GetStaticMesh());
}