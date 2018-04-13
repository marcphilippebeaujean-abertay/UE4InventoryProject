// Fill out your copyright notice in the Description page of Project Settings.

#include "LanternItemComponent.h"


// Sets default values for this component's properties
ULanternItemComponent::ULanternItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULanternItemComponent::BeginPlay()
{
	Super::BeginPlay();
	// Find the light component in the player's components
	LanternLight = GetOwner()->FindComponentByClass<UPointLightComponent>();
	if (!LanternLight)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find lantern spot light!"));
		return;
	}
	
	this->SetComponentTickEnabled(false);

	// ...
	
}

// Called every frame
void ULanternItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(LanternLight)
	{
		// Decrement attenuation of light
		if(CurAttenuation > 0)
		{
			CurAttenuation -= (LightDecrementRate * DeltaTime);
			if(CurAttenuation < 8)
			{
				CurAttenuation = 8.0f;
			}
			LanternLight->SetAttenuationRadius(CurAttenuation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find lantern spot light!"));
		return;
	}

	// ...
}

void ULanternItemComponent::RelightLantern()
{
	// Reset attenuation for the point light
	CurAttenuation = InitAttenuation;
	LanternLight->SetAttenuationRadius(CurAttenuation);
}

void ULanternItemComponent::ExtinguishLantern()
{
	// Reset lantern attenuation
	CurAttenuation = 8;
	LanternLight->SetAttenuationRadius(CurAttenuation);
}
