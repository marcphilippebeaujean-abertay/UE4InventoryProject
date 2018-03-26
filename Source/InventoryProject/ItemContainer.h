// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollectableObject.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ItemContainer.generated.h"


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

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Add object to the inventory
	void AddObjToContainer(ACollectableObject* NewItem);

	// Access the widget of the container
	TSubclassOf<class UUserWidget> GetContainerWidget() { return ContainerWidgetClass; }

	// Returns all items within the contain
	TArray<ACollectableObject*> GetContainerItems() { return ContainerItems; };

private:
	// Array that stores objects which are in the container
	TArray<ACollectableObject*> ContainerItems;

	// Item that is used for empty slots in the inventory - makes it easier to switch between item slots and allows for more customisation by the designer
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ACollectableObject> EmptySlotClass;
	ACollectableObject* EmptySlot = nullptr;

	// Maximum number of different classes that the inventory can hold - needs to be synchronised with the UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Container", Meta = (AllowPrivateAccess = "true"))
	int MaxItemSlots = 10;

	// Widget that is displayed when the user accesses the container
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Visual", Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> ContainerWidgetClass;
};
