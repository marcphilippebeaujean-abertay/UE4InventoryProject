// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultEmptySlot.h"

void ADefaultEmptySlot::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	// Define the variables for the empty slot
	IndicatorDisplayName = "Empty Slot";
	bEmptySlot = true;
}


