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
	TSubclassOf<ALSWeaponPistol> PistolClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf<ALSWeaponShotgun> ShotgunClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf< ALSWeaponRifle> RifleClass; 

	UPROPERTY(EditAnywhere, Category="Weapons") 
	UInputAction* IA_EquipPistol; 
	UPROPERTY(EditAnywhere, Category="Weapons") 
	UInputAction* IA_EquipShotgun; 
	UPROPERTY(EditAnywhere, Category="Weapons") 
	UInputAction* IA_EquipRifle; 

	UPROPERTY(EditAnywhere, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> WeaponToSpawn; 

	void EquipWeapon(int WeaponType); 
	void EquipPistol(); 
	void EquipShotgun(); 
	void EquipRifle(); 
}; 
 