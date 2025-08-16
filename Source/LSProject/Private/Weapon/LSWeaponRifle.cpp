#include "Weapon/LSWeaponRifle.h"

ALSWeaponRifle::ALSWeaponRifle()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MaxAmmo = 30;
	CurrentAmmo = 0;
	Damage = 50.0f;
	FireRate = 0.1f;
}

void ALSWeaponRifle::BeginPlay()
{
	Super::BeginPlay();
}

void ALSWeaponRifle::Fire()
{
	Super::Fire();
}

void ALSWeaponRifle::Reload()
{
	Super::Reload();
}


