#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Character/LSPlayerCharacter.h"
#include "Weapon/LSWeaponBase.h"

ULSPlayerWeaponSystemComp::ULSPlayerWeaponSystemComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentWeapon = nullptr;
	IsArmedWeapon = false;
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

void ULSPlayerWeaponSystemComp::UnEquipWeapon()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}
}
