// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectableObject.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ItemContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryInterfaceUpdate, const TArray<ACollectableObject*>&, Contents);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INVENTORYPROJECT_API UItemContainer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemContainer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Array that stores objects which are in the container
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
	void SwapItems(UItemContainer* OtherContainer, int OtherItemID, int LocalItemID);

	// Sets an item in the container
	void SetContainerItem(int ContainerID, ACollectableObject* NewItem) { ContainerItems[ContainerID] = NewItem; }

	// Get an item from a container
	ACollectableObject* GetContainerItem(int ContainerID) { return ContainerItems[ContainerID]; }

	// Maximum number of different classes that the inventory can hold - needs to be synchronised with the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container")
	int MaxItemSlots = 10;

	bool IsQuickAccess() { return bQuickAccess; }

	// Broadcasts event, indicating that the user interface needs to be updated
	void BroadcastWidgetUpdate();

private:

	// Item that is used for empty slots in the inventory - makes it easier to switch between item slots and allows for more customisation by the designer
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACollectableObject> EmptySlotClass;
	ACollectableObject* EmptySlot = nullptr;

	// Delegate used to broadcast to the blueprint that the invnetory has been updated
	UPROPERTY(BlueprintAssignable, Category = "UI Events")
	FInventoryInterfaceUpdate OnUpdateContainer;
};
