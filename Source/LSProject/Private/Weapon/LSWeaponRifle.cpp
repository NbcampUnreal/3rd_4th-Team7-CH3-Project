// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSWeaponRifle.h"


ALSWeaponRifle::ALSWeaponRifle()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshAsset(TEXT("/Game/Weapons/Rifle/Mesh/SM_Rifle.SM_Rifle"));
	if (StaticMeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshAsset.Object);
	}
}


