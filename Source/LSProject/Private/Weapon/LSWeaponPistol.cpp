#include "Weapon/LSWeaponPistol.h"
#include "Weapon/LSWeaponBase.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

ALSWeaponPistol::ALSWeaponPistol()
{
	PrimaryActorTick.bCanEverTick = false;

	MaxAmmo = 15;
	CurrentAmmo = 0;
	Damage = 30.0f;
	FireRate = 0.2f;
}

void ALSWeaponPistol::Fire()
{
	Super::Fire();
}

void ALSWeaponPistol::Reload()
{
	Super::Reload();
}
