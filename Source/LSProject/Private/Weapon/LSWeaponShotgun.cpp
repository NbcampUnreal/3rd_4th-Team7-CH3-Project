// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSWeaponShotgun.h"

ALSWeaponShotgun::ALSWeaponShotgun()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshAsset(TEXT("/Game/Weapons/Shotgun/Mesh/SM_Shotgun.SM_Shotgun"));
	if (StaticMeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshAsset.Object);
	}
}

