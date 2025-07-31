// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"              
#include "UObject/ConstructorHelpers.h"
#include "LSWeaponBase.generated.h"

UCLASS()
class LSPROJECT_API ALSWeaponBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALSWeaponBase();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* Scene;
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
//	USphereComponent* Collision;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	UStaticMeshComponent* StaticMesh;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
