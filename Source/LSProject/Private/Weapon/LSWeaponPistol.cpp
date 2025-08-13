// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/LSWeaponPistol.h"
#include "Weapon/LSWeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"                
#include "NiagaraFunctionLibrary.h"
#include "Weapon/LSPlayerWeaponSystemComp.h"


ALSWeaponPistol::ALSWeaponPistol()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALSWeaponPistol::Fire()
{
	Super::Fire();
	
	// ULSPlayerWeaponSystemComp* WeaponComp = GetOwner() ? GetOwner()->FindComponentByClass<ULSPlayerWeaponSystemComp>() : nullptr;
	// if (!WeaponComp) return;
	//
	// FHitResult Hit;
	// bool bHit = WeaponComp->PerformLineTrace(Damage, FireRange, Hit);
	//
	// if (MuzzleEffect && StaticMesh)
	// {
	// 	UNiagaraFunctionLibrary::SpawnSystemAttached(
	// 		MuzzleEffect,
	// 		StaticMesh,
	// 		TEXT("Muzzle"),
	// 		FVector::ZeroVector,
	// 		FRotator::ZeroRotator,
	// 		EAttachLocation::SnapToTarget,
	// 		true
	// 	);
	// }
	//
	// if (bHit && ImpactEffect)
	// {
	// 	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
	// 		GetWorld(),
	// 		ImpactEffect,
	// 		Hit.ImpactPoint,
	// 		Hit.ImpactNormal.Rotation()
	// 	);
	// }
}
