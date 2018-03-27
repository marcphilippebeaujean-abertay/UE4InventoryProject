// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickAccess.h"


ACollectableObject* UQuickAccess::UpdateSelectedItem(bool increment)
{
	// Manipulate selected value item based on condition
	if(increment)
	{
		CurSelectedItem++;
	}
	else
	{
		CurSelectedItem--;
	}
	// Ensure that cur selected item is going to be within the container's array - if they exceed them, return the highest/lowest possible value
	if(CurSelectedItem > MaxItemSlots)
	{
		CurSelectedItem = 0;
	}
	if(CurSelectedItem < 0)
	{
		CurSelectedItem = MaxItemSlots;
	}
	// Return the item from the array
	UE_LOG(LogTemp, Error, TEXT("Currently selected item in quick access is now %s!"), *ContainerItems[CurSelectedItem]->GetIndicatorName());
	return ContainerItems[CurSelectedItem];
}
