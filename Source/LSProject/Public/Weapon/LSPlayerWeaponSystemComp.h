#pragma once 

#include "CoreMinimal.h" 
#include "Weapon/LSWeaponBase.h" 
#include "Weapon/LSWeaponPistol.h"  
#include "Weapon/LSWeaponShotgun.h" 
#include "Weapon/LSWeaponRifle.h" 
#include "Components/ActorComponent.h" 
#include "LSPlayerWeaponSystemComp.generated.h" 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) 
class LSPROJECT_API ULSPlayerWeaponSystemComp : public UActorComponent 
{ 
	GENERATED_BODY() 

public: 
	ULSPlayerWeaponSystemComp(); 

	UPROPERTY() 
	ALSWeaponBase* CurrentWeapon; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> PistolClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> ShotgunClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf< ALSWeaponBase> RifleClass; 
/*
	UPROPERTY(EditAnywhere, Category="Weapons") 
	UInputAction* IA_EquipPistol; 
	UPROPERTY(EditAnywhere, Category="Weapons") 
	UInputAction* IA_EquipShotgun; 
	UPROPERTY(EditAnywhere, Category="Weapons") 
	UInputAction* IA_EquipRifle; 
*/
	UPROPERTY(EditAnywhere, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> WeaponToSpawn;

	
	UFUNCTION()
	void EquipWeapon(int WeaponType);
	UFUNCTION()
	void EquipPistol();
	UFUNCTION()
	void EquipShotgun();
	UFUNCTION()
	void EquipRifle();
	UFUNCTION()
	void FireWeapon();

protected:
	virtual void BeginPlay() override;
}; 
 