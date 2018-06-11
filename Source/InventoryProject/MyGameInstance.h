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

	class UPlayerInventory* GetPlayerInventory()
	{
		return m_playerInventory;
	}

	class UQuickAccess* GetPlayerQuickAccess()
	{
		return m_playerQuickAccess;
	}

	void SetPlayerInventory(class UPlayerInventory* l_itemContainer);
	void SetPlayerQuickAccess(class UQuickAccess* l_itemContainer);
	
private:

	// References to the player's item containers
	UPROPERTY()
	class UPlayerInventory* m_playerInventory = nullptr;
	UPROPERTY()
	class UQuickAccess* m_playerQuickAccess = nullptr;
	

};
