// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"               
#include "UObject/ConstructorHelpers.h" 
#include "LSWeaponBase.generated.h" 


UCLASS() 
class LSPROJECT_API ALSWeaponBase : public AActor 
{ 
	GENERATED_BODY() 

public: 
	ALSWeaponBase(); 

protected:
	/*
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State");
	float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State");
	float FireRange;
*/
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Components") 
	USceneComponent* Scene; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Components")  
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Components")
	USkeletalMeshComponent* WeaponSkeletalMesh;
}; 
