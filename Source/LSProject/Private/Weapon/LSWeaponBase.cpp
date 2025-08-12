// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/LSWeaponBase.h"
#include "GameFramework/Actor.h"

// Sets default values
ALSWeaponBase::ALSWeaponBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene); // 임시로
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponStaticMesh"));
	WeaponSkeletalMesh->SetupAttachment(Scene);

	Damage = 20.0f;
	FireRange = 100000.0f;
	
}

void ALSWeaponBase::OnFire()
{
	
}







