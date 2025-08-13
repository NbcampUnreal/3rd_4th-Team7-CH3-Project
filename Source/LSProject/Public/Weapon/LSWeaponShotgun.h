// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "LSWeaponShotgun.generated.h"

/**
 * 
 */
UCLASS()
class LSPROJECT_API ALSWeaponShotgun : public ALSWeaponBase
{
	GENERATED_BODY()

public:
	ALSWeaponShotgun();

protected:
	virtual void Fire() override;
};
