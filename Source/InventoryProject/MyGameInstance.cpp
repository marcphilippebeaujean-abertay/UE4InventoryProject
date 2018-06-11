// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "QuickAccess.h"
#include "PlayerInventory.h"

void UMyGameInstance::SetPlayerInventory(UPlayerInventory* l_itemContainer)
{
	m_playerInventory = l_itemContainer;
}

void UMyGameInstance::SetPlayerQuickAccess(UQuickAccess* l_itemContainer)
{
	m_playerQuickAccess = l_itemContainer;
}
