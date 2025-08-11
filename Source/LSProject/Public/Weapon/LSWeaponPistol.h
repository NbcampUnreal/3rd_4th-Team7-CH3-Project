// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "InputActionValue.h"
#include "NiagaraSystem.h"                
#include "NiagaraFunctionLibrary.h"
#include "LSWeaponPistol.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class LSPROJECT_API ALSWeaponPistol : public ALSWeaponBase
{
	GENERATED_BODY()
	
public:
	ALSWeaponPistol();

protected:
//	virtual void BeginPlay() override;
	
	void OnEquip(const FInputActionValue& Value);
	void OnFire(const FInputActionValue& Value);

	void EquipWeapon();
//	void Fire();
	void PerformLineTrace();
	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	float FireRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	UNiagaraSystem* MuzzleEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	UNiagaraSystem* ImpactEffect;
};
