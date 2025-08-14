// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSPlayerState.h"

ALSPlayerState::ALSPlayerState()
{
	//추후 수정
	Coin=1000;
}

void ALSPlayerState::AddZombieKill(int32 Delta)
{
	ZombieNum = FMath::Max(0, ZombieNum + Delta);
}

void ALSPlayerState::AddCoin(int32 NewCoin)
{
	Coin+=NewCoin;
}
