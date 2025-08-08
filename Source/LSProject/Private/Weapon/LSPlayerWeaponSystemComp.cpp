#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/LSWeaponBase.h"

ULSPlayerWeaponSystemComp::ULSPlayerWeaponSystemComp()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	CurrentWeapon = nullptr;
	WeaponToSpawn = nullptr;
}

void ULSPlayerWeaponSystemComp::EquipWeapon(int WeaponType)
{
	// 기존 무기 제거
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	switch (WeaponType)
	{
	case 1:
		WeaponToSpawn = PistolClass;
		break;
	case 2:
		WeaponToSpawn = ShotgunClass;
		break;
	case 3:
		WeaponToSpawn = RifleClass;
		break;
	default:
		return;
	}

	if (WeaponToSpawn)
	{
		UWorld* World = GetWorld();
		if (!World) return;

		AActor* OwnerActor = GetOwner();
		if (!OwnerActor) return;
		
		CurrentWeapon = World->SpawnActor<ALSWeaponBase>(WeaponToSpawn);
		if (CurrentWeapon)
		{
			CurrentWeapon->SetOwner(OwnerActor);

			ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
			if (OwnerCharacter && CurrentWeapon)
			{
				USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh();
				if (MeshComp)
				{
					CurrentWeapon->AttachToComponent(
					MeshComp,
					FAttachmentTransformRules::SnapToTargetIncludingScale,
					FName("RightWeapon")
					);
				}
			}
		}
	}
}

void ULSPlayerWeaponSystemComp::EquipPistol()
{
	EquipWeapon(1);
}

void ULSPlayerWeaponSystemComp::EquipShotgun()
{
	EquipWeapon(2);
}

void ULSPlayerWeaponSystemComp::EquipRifle()
{
	EquipWeapon(3);
}
