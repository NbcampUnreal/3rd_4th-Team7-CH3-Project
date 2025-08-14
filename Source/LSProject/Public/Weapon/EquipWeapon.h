// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "EquipWeapon.generated.h"

/**
 * 
 */
UCLASS()
class LSPROJECT_API UEquipWeapon : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp,
	                    UAnimSequenceBase* Animation,
	                    const FAnimNotifyEventReference& EventReference) override;
};
