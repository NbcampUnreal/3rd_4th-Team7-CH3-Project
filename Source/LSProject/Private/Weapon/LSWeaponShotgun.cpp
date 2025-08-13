// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSWeaponShotgun.h"

ALSWeaponShotgun::ALSWeaponShotgun()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ALSWeaponShotgun::Fire()
{
	Super::Fire();
}

