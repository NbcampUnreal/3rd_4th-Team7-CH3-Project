// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/LSPlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "LSInventoryComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LSPROJECT_API ULSInventoryComp : public UActorComponent
{
	GENERATED_BODY()

public:

	ULSInventoryComp();
protected:
	virtual void BeginPlay() override;

public:
	void AddToInven(const FName& Input,int32 Amount);
	int32 CountItem(const FName& Input);
	void Equip(const FName& Input);
	void Unequip();
	void ChangeWeaponSlot(const FName& NewWeapon);
	FORCEINLINE FName GetMyWeapon() const {return MyWeaponName;}
	bool HasAmmo();
	int RequiredAmmo(int32 RequiredAmmo);
	void AddAmmoToInven(int32 RequiredAmmo);
	
private:
	UPROPERTY()
	TMap<FName,int32> MyItems;

	FName MyWeaponName;
	
	ECurrentWeapon ChangeWeaponNameToEnum(const FName& Input);
	void EquipWeapon();
};
