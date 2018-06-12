// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "QuickAccess.h"
#include "PlayerInventory.h"
#include "MySaveGame.h"

UMyGameInstance::UMyGameInstance()
{
	// Clear inventory for the demonstration, when game begins
	UMySaveGame* SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
	SaveGameInstance = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameInstance->GetSaveSlotName(), SaveGameInstance->GetUserIndex()));
	SaveGameInstance->ClearPlayerContainerRefs();
	UE_LOG(LogTemp, Error, TEXT("Reset player container references!"));
	// Save to slot, so that it can be recalled later
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->GetSaveSlotName(), SaveGameInstance->GetUserIndex());
}
