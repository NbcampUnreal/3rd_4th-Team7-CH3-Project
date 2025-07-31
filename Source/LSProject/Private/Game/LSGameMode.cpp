// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSGameMode.h"
#include "Game/LSPlayerState.h"

ALSGameMode::ALSGameMode()
{
	PlayerStateClass = ALSPlayerState::StaticClass();
}
