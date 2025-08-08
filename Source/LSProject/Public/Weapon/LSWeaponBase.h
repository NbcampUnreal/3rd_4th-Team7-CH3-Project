// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"              
#include "UObject/ConstructorHelpers.h"
#include "LSWeaponBase.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FOnWeaponEquipped, int32, WeaponTypeID);

UCLASS()
class LSPROJECT_API ALSWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	ALSWeaponBase();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* Scene;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	UStaticMeshComponent* StaticMesh;
};
