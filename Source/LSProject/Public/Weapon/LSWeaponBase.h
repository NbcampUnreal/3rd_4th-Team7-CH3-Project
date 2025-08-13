// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "WeaponInterface.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"               
#include "UObject/ConstructorHelpers.h"
#include "LSWeaponBase.generated.h" 

class UNiagaraComponent;

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

	UFUNCTION(BlueprintCallable, Category = "Socket")
	FTransform GetLeftHandSocketTransform() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State");
	float Damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon State");
	float FireRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	UNiagaraComponent* Niagara;
		
	

}; 
