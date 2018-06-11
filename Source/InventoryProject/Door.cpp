// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "PlayerInventory.h"
#include "FirstPersonCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ADoor::OnInteracted()
{
	
}

bool ADoor::CanInteract()
{
	if(PlayerRef == nullptr)
	{
		PlayerRef = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}


	return true;
}
