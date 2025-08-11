// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSGameState.h"

ALSGameState::ALSGameState()
{
	bIsCharacterOverlappedWithDoor=false;
	bIsDay=true;
	//bIShowingShopUI=false;
}

bool ALSGameState::bGetCanOpenShopUI()
{
	if (bIsCharacterOverlappedWithDoor && bIsDay)
	{
		return true;
	}
	
	return false;
}
