// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "LSWeaponBase.generated.h"

class UNiagaraSystem;

UCLASS()
class LSPROJECT_API ALSWeaponBase : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	ALSWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponScene")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "WeaponMesh")
	USkeletalMeshComponent* WeaponSkeletalMesh;

	virtual void Fire() override;

	float GetFireRate() const;

	UFUNCTION(BlueprintCallable, Category = "Socket")
	FTransform GetLeftHandSocketTransform() const;

protected:
	int32 MaxAmmo;
	int32 CurrentAmmo;

	float FireRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UNiagaraSystem* FireEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireSoundVolume;
};
