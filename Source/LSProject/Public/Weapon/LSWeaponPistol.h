// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "NiagaraSystem.h"                
#include "NiagaraFunctionLibrary.h"
#include "LSWeaponPistol.generated.h"

class UNiagaraSystem;
class ULSPlayerWeaponSystemComp;

UCLASS()
class LSPROJECT_API ALSWeaponPistol : public ALSWeaponBase
{
	GENERATED_BODY()
	
public:
	ALSWeaponPistol();

protected:
//	virtual void BeginPlay() override;
	
	void OnFire();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	UNiagaraSystem* MuzzleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	UNiagaraSystem* ImpactEffect;
};
