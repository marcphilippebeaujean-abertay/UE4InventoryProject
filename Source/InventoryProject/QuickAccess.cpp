// Fill out your copyright notice in the Description page of Project Settings.

#include "QuickAccess.h"


// Sets default values for this component's properties
void UQuickAccess::OnComponentCreated()
{
	Super::OnComponentCreated();

	// Set quick access check to true
	bQuickAccess = true;
}

void UQuickAccess::UpdateSelectedItem(bool increment)
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
	if(CurSelectedItem > (MaxItemSlots - 1))
	{
		CurSelectedItem = 0;
	}
	if(CurSelectedItem < 0)
	{
		CurSelectedItem = (MaxItemSlots - 1);
	}
	BroadcastWidgetUpdate();
}
