// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSPlayerState.h"

#include "Kismet/GameplayStatics.h"

ALSPlayerState::ALSPlayerState()
{
	EmptySound=nullptr;
	Coin=500;
}



void ALSPlayerState::AddZombieKill(int32 Delta)
{
	ZombieNum = FMath::Max(0, ZombieNum + Delta);
}

bool ALSPlayerState::AddCoin(int32 NewCoin)
{
	if (Coin+NewCoin >= 0)
	{
		Coin+=NewCoin;
		return true;
	}
	if (EmptySound)
	{
		UGameplayStatics::PlaySound2D(this, EmptySound);
	}
	return false;
}
