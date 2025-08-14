
#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "LSWeaponPistol.generated.h"

UCLASS()
class LSPROJECT_API ALSWeaponPistol : public ALSWeaponBase
{
	GENERATED_BODY()
	
public:
	ALSWeaponPistol();

protected:
	
	virtual void Fire() override;
	
};
