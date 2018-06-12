// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
*
*/
UCLASS()
class INVENTORYPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	// Stores the containres in the save game object
	void UpdatePlayerContainers(class UPlayerInventory* l_plInventory, class UQuickAccess* l_quickAccess);

	bool SaveDataAvailable()
	{
		return m_initialisedSaveFile;
	}

private:

	bool m_initialisedSaveFile = false;
};