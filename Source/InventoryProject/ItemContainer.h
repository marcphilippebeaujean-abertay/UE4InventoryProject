// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectableObject.h"
#include "Engine/World.h"
#include "DefaultEmptySlot.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ItemContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FContentsInterfaceUpdate, const TArray<ACollectableObject*>&, Contents);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class INVENTORYPROJECT_API UItemContainer : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UItemContainer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Array of items that can be set in the blueprint, which dictates what items are in the container at game start (or when the player interacts with it)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container", Meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ACollectableObject>> InitItems;

	// Array that stores objects which are in the container
	UPROPERTY()
	TArray<ACollectableObject*> ContainerItems;

	// Variable used to differentiate between quick access and regular containers, without having to cast
	bool bQuickAccess = false;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Add object to the inventory
	void AddObjToContainer(ACollectableObject* NewItem);

	// Returns all items within the contain
	TArray<ACollectableObject*> GetContainerItems() { return ContainerItems; }; 

	// Allows the items to be swapped between containers
	UFUNCTION(BlueprintCallable)
	virtual void SwapItems(UItemContainer* OtherContainer, int OtherItemID, int LocalItemID);

	// Sets an item in the container
	virtual void SetContainerItem(int ContainerID, ACollectableObject* NewItem)
	{
		ContainerItems[ContainerID] = NewItem;
		CheckForDepletedItems();
	}

	// Removes items that the player owns from the inventory
	UFUNCTION(BlueprintCallable)
	virtual void DropItem(int ItemID);

	// Get an item from a container
	ACollectableObject* GetContainerItem(int ContainerID) { return ContainerItems[ContainerID]; }

	// Broadcasts event, indicating that the user interface needs to be updated
	void BroadcastWidgetUpdate();

	// Create reference to empty slot object
	void InitContainerContents(ACollectableObject* EmptySlotClass);

	// Get empty slot
	ACollectableObject* GetEmptySlot() { return EmptySlot; }

	// Find an object in the inventory that stores the required type
	ACollectableObject* GetResourceOfType(EResourceType ResType);

	// Sets objects that have been depleted to type "EmptySlot"
	void CheckForDepletedItems();

	// Getters
	UFUNCTION(BlueprintCallable)
	int GetNumOfRows() { return NumberOfRows; }
	UFUNCTION(BlueprintCallable)
	int GetNumOfCollumns() { return NumberOfCollumns; }
	UFUNCTION(BlueprintCallable)
	int GetMaxItemSlots() { return (NumberOfCollumns * NumberOfRows); }
	bool IsQuickAccess() { return bQuickAccess; }
	bool IsUnitinitialised() { return bUninitialised; }

private:
	// Reference to empty slot item
	ACollectableObject* EmptySlot = nullptr;

	// Define dimensions of the inventory
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container", Meta = (AllowPrivateAccess = "true", UIMin = '1', UIMax = '8'))
	int NumberOfCollumns = 4;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container", Meta = (AllowPrivateAccess = "true", UIMin = '1', UIMax = '8'))
	int NumberOfRows = 2;

	// Delegate used to broadcast to the blueprint that the invnetory has been updated
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FContentsInterfaceUpdate OnUpdateContainer;

	// Condition that checks if the container has been initialised
	bool bUninitialised = true;
};