// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultEmptySlot.h"

void ADefaultEmptySlot::OnConstruction(const FTransform & Transform)
{
	Super::Init();
	// Define the variables for the empty slot
	IndicatorDisplayName = "Empty Slot";
	bEmptySlot = true;
	bEquipable = true;
	UE_LOG(LogTemp, Warning, TEXT("Init empty slot called!"));
}


