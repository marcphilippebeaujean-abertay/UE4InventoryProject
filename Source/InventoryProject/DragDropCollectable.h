// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CollectableObject.h"
#include "DragDropCollectable.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API UDragDropCollectable : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	/** Allows for storing and transferring items when dragging them in the inventory. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true"))
	ACollectableObject* InventoryItem;
	
	
};
