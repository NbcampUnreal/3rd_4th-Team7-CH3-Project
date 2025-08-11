// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/LSWeaponPistol.h"
#include "Weapon/LSWeaponBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "NiagaraSystem.h"                
#include "NiagaraFunctionLibrary.h"


ALSWeaponPistol::ALSWeaponPistol()
{
	PrimaryActorTick.bCanEverTick = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>StaticMeshAsset(TEXT("/Game/Weapons/Pistol/Mesh/SM_Pistol.SM_Pistol"));
	if (StaticMeshAsset.Succeeded())
	{
		StaticMesh->SetStaticMesh(StaticMeshAsset.Object);

		UE_LOG(LogTemp, Warning, TEXT("물체 생성"));
	}

}

void ALSWeaponPistol::OnFire(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("사격"));
	PerformLineTrace();
}

void ALSWeaponPistol::PerformLineTrace()
{
	AActor* MyOwner = GetOwner();
	if (!MyOwner) return;

	FVector MuzzleLocation = StaticMesh->GetSocketLocation(TEXT("Muzzle"));
	FRotator MuzzleRotation = StaticMesh->GetSocketRotation(TEXT("Muzzle"));
	UE_LOG(LogTemp, Warning, TEXT("머즐 위치 : %s"), *MuzzleLocation.ToString());
//	MyOwner->GetActorEyesViewPoint(MuzzleLocation, MuzzleLocation);
	
	FVector ShotDirection = MuzzleRotation.Vector();
	FVector TraceStart = MuzzleLocation;
	FVector TraceEnd = TraceStart + (ShotDirection * FireRange);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(MyOwner);
	Params.AddIgnoredActor(this);
	Params.bTraceComplex = true;

	if (GetWorld()->LineTraceSingleByChannel(
		Hit,
		MuzzleLocation,
		TraceEnd,
		ECC_Visibility,
		Params	
		)
	)
	{
		if (AActor* HitActor = Hit.GetActor())
		{
			UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, MyOwner->GetInstigatorController(), this, nullptr);
		}

		if (ImpactEffect)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ImpactEffect,
				Hit.ImpactPoint,
				Hit.ImpactNormal.Rotation()
			);
		}

		TraceEnd = Hit.ImpactPoint;
	}

	
	PlayFireEffects(TraceEnd);
}

void ALSWeaponPistol::PlayFireEffects(FVector TraceEnd)
{
	if (MuzzleEffect && StaticMesh)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			MuzzleEffect,
			StaticMesh,
			TEXT("Muzzle"),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			true
		);
	}
//	DrawDebugPoint(GetWorld(), TraceEnd, 10.0f, FColor::Red, true); // test용이래!
}

	



