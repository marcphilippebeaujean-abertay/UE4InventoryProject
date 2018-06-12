// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CollectableObject.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
USTRUCT()
struct FInventoryObjectsStruct
{
	GENERATED_BODY()
private:
	// Blueprint class of the object
	UPROPERTY()
	TSubclassOf<ACollectableObject> m_collectableClass = nullptr;
	
	// Number of units contained by the object before it was destroyed
	UPROPERTY()
	uint32 m_unitCount = 1;

	// Index of the object when it was deleted
	UPROPERTY()
	uint32 m_inventoryIndex = 1;
public:

	void SetCollectableClass(TSubclassOf<ACollectableObject> l_collectableClass)
	{
		m_collectableClass = l_collectableClass;
	}
	TSubclassOf<ACollectableObject> GetCollectableClass()
	{
		return m_collectableClass;
	}

	void SetUnitCount(int l_unitCount)
	{
		m_unitCount = l_unitCount;
	}
	uint32 GetUnitCount()
	{
		return m_unitCount;
	}

	void SetInventoryIndex(int l_index)
	{
		m_inventoryIndex = l_index;
	}
	uint32 GetInventoryIndex()
	{
		return m_inventoryIndex;
	}
};

UCLASS()
class INVENTORYPROJECT_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UMySaveGame();

	FString GetSaveSlotName()
	{
		return m_saveSlotName;
	}

	uint32 GetUserIndex()
	{
		return m_userIndex;
	}

	void SaveContainerItems(FString l_containerID, class UItemContainer* l_itemContainer);


private:

	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString m_saveSlotName;

	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 m_userIndex;

	// Items stored when saving and changing levels, that need to be reinstantiated
	TMap<FString, TArray<FInventoryObjectsStruct>> m_containerItems;
};
