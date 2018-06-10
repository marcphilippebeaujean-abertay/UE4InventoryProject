// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "CollectableObject.h"
#include "ItemContainer.h"
#include "DragDropCollectable.generated.h"

/**
 * 
 */
UCLASS()
class INVENTORYPROJECT_API UDragDropCollectable : public UDragDropOperation
{
	GENERATED_BODY()
	
public:

	/** Sends the item container corresponding to the dragged item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true"))
	UItemContainer* DraggedItemContainer;
	
	/** Sends the item ID corresponding to the dragged item. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drag and Drop", meta = (ExposeOnSpawn = "true"))
	int DraggedItemID;
};
