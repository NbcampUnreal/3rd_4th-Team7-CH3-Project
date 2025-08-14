
#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "LSWeaponRifle.generated.h"

UCLASS()
class LSPROJECT_API ALSWeaponRifle : public ALSWeaponBase
{
	GENERATED_BODY()
	
public:
	ALSWeaponRifle();

protected:
	virtual void Fire() override;
	virtual void Reload() override;
};
