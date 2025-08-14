#include "Weapon/LSWeaponRifle.h"

ALSWeaponRifle::ALSWeaponRifle()
{
	PrimaryActorTick.bCanEverTick = false;
	
	MaxAmmo = 30;
	CurrentAmmo = MaxAmmo;
	Damage = 50.0f;
	FireRate = 0.1f;
}

void ALSWeaponRifle::Fire()
{
	Super::Fire();
}


