// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipableObject.h"


void AEquipableObject::Init()
{
	// Set equipable object to true
	bEquipable = true;
	UE_LOG(LogTemp, Warning, TEXT("Init equipable called!"));

	Super::Init();
}


