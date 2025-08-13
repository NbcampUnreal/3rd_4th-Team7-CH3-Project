#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Character/LSPlayerCharacter.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/LSWeaponBase.h"

ULSPlayerWeaponSystemComp::ULSPlayerWeaponSystemComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentWeapon = nullptr;
}

void ULSPlayerWeaponSystemComp::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ALSPlayerCharacter>(GetOwner());
}

void ULSPlayerWeaponSystemComp::EquipWeapon(int Index)
{
	if (WeaponClass.IsEmpty()) return;

	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	if (OwnerCharacter)
	{
		if (WeaponClass.IsValidIndex(Index))
		{
			CurrentWeapon = Cast<ALSWeaponBase>(GetWorld()->SpawnActor(WeaponClass[Index]));
			CurrentWeapon->SetOwner(OwnerCharacter);
			UE_LOG(LogTemp, Warning, TEXT("weapon spawn"));
			if (USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh())
			{
				IsArmedWeapon = CurrentWeapon->AttachToComponent(
					Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					TEXT("RightWeapon"));
			}
		}
	}
}

bool ULSPlayerWeaponSystemComp::PerformLineTrace(float Damage, float FireRange, FHitResult& OutHit)
{
	// ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	// if (!OwnerCharacter) return false;
	//
	// UStaticMeshComponent* WeaponMesh = CurrentWeapon->FindComponentByClass<UStaticMeshComponent>();
	// if (!WeaponMesh) return false;
	//
	// APlayerController* PC = Cast<APlayerController>(OwnerCharacter->GetController());
	// if (!PC) return false;
	//
	// FVector MuzzleLocation = WeaponMesh->GetSocketLocation("Muzzle");
	// FRotator MuzzleRotation = WeaponMesh->GetSocketRotation("Muzzle");
	//
	// FVector TraceStart = MuzzleLocation;
	// FVector TraceEnd = TraceStart + (MuzzleRotation.Vector() * FireRange);
	//
	// FCollisionQueryParams Params;
	// Params.AddIgnoredActor(OwnerCharacter);
	// if (CurrentWeapon) Params.AddIgnoredActor(CurrentWeapon);
	// Params.bTraceComplex = true;

	// 	bool bHit = GetWorld()->LineTraceSingleByChannel(
	// 		OutHit,
	// 		TraceStart,
	// 		TraceEnd,
	// 		ECC_Visibility,
	// 		Params
	// 	);
	// 	
	// #if WITH_EDITOR
	// 	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 3.0f, 5, 1.0f);
	// 	FVector MuzzleForwardVector = WeaponMesh->GetSocketTransform("Muzzle").GetUnitAxis(EAxis::X);
	// 	DrawDebugCoordinateSystem(GetWorld(), MuzzleLocation, WeaponMesh->GetSocketRotation("Muzzle"), 20.f, false, 5.f);
	// 	DrawDebugLine(GetWorld(), MuzzleLocation, TraceEnd, FColor::Green, false, 5.f, 0, 2.f);
	// #endif
	//
	// 	if (bHit && OutHit.GetActor())
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("라인트레이스 명중: %s"), *OutHit.GetActor()->GetName());
	// 		
	// 		UGameplayStatics::ApplyDamage(
	// 			OutHit.GetActor(),
	// 			Damage,
	// 			PC,
	// 			OwnerCharacter,
	// 			nullptr
	// 		);
	// 	}
	//
	return true;
}
