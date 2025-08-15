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

void ULSPlayerWeaponSystemComp::EquipWeapon()
{
	if (WeaponClass.IsEmpty()) return;
	
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	} 

	if (OwnerCharacter)
	{
		const int32 Index = static_cast<int32>(OwnerCharacter->GetCurrentWeapon()) - 1;
		
		if (WeaponClass.IsValidIndex(Index))
		{
			CurrentWeapon = Cast<ALSWeaponBase>(GetWorld()->SpawnActor(WeaponClass[Index]));
			CurrentWeapon->SetOwner(OwnerCharacter);
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
