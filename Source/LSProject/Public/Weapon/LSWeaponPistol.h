// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "NiagaraFunctionLibrary.h"
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
