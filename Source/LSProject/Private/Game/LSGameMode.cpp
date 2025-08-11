// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSGameMode.h"
#include "Game/LSPlayerState.h"
#include "Controller/LSPlayerController.h"
#include "Game/LSGameState.h"

ALSGameMode::ALSGameMode()
{
	PlayerStateClass = ALSPlayerState::StaticClass();
	PlayerControllerClass = ALSPlayerController::StaticClass();
	GameStateClass=ALSGameState::StaticClass();
}
