// Fill out your copyright notice in the Description page of Project Settings.

#include "ResourceObject.h"


void AResourceObject::Init()
{
	Super::Init();

	// Set initial resources for object
	CurResourceCount = InitialResources;
}

