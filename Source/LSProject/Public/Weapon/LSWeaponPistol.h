// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/LSWeaponBase.h"
#include "InputActionValue.h"
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
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* WeaponMappingContext;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_Fire;
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* IA_EquipPistol;

	void OnEquip(const FInputActionValue& Value);
	void OnFire(const FInputActionValue& Value);

	void EquipWeapon();
//	void Fire();
	void PerformLineTrace();
	void PlayFireEffects(FVector TraceEnd);

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float Damage;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	float FireRange;
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UParticleSystem* MuzzleEffect;\
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UParticleSystem* ImpactEffect;
};
