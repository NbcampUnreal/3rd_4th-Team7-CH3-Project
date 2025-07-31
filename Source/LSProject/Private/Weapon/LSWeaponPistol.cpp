// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/LSWeaponPistol.h"
#include "Weapon/LSWeaponBase.h"

ALSWeaponPistol::ALSWeaponPistol()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshAsset(TEXT("/Game/Weapons/Pistol/Mesh/SM_Pistol.SM_Pistol"));
	if (StaticMeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshAsset.Object);
	}
	
}


