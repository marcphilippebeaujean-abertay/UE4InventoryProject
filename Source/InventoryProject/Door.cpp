// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "PlayerInventory.h"
#include "FirstPersonCharacter.h"
#include "KeyCard.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	// Get player reference
	PlayerRef = Cast<AFirstPersonCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void ADoor::OnInteracted()
{
	
}

bool ADoor::CanInteract()
{
	// Check if player reference exists
	if(PlayerRef == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Couldn't find reference to player!"));
		return false;
	}
	// Get inventory reference
	UPlayerInventory* PlayerInventory = PlayerRef->FindComponentByClass<UPlayerInventory>();
	// Check if there are keys in the inventory
	TArray<ACollectableObject*> KeyObjects = PlayerInventory->GetResourcesOfType(EResourceType::Key);
	if(KeyObjects.Num() > 0)
	{
		// Look for the right key type
		for(auto &itr : KeyObjects)
		{
			// Get key type of current object
			EKeyType KeyType = Cast<AKeyCard>(itr)->GetKeyType();
			if(KeyType == RequiredKey || KeyType == EKeyType::Master)
			{
				UE_LOG(LogTemp, Warning, TEXT("Can interact with door!"));
				return true;
			}
		}
	}
	return false;
}
