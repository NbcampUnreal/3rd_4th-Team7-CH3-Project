#pragma once 

#include "CoreMinimal.h" 
#include "Weapon/LSWeaponBase.h" 
#include "Components/ActorComponent.h" 
#include "LSPlayerWeaponSystemComp.generated.h" 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) 
class LSPROJECT_API ULSPlayerWeaponSystemComp : public UActorComponent 
{ 
	GENERATED_BODY() 

public: 
	ULSPlayerWeaponSystemComp();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> PistolClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> ShotgunClass; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapons") 
	TSubclassOf< ALSWeaponBase> RifleClass; 
	UPROPERTY(EditAnywhere, Category="Weapons") 
	TSubclassOf<ALSWeaponBase> WeaponToSpawn;

	UPROPERTY() 
	ALSWeaponBase* CurrentWeapon; 
	
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

	bool PerformLineTrace(float Damage, float FireRange, FHitResult& OutHit);

protected:
	virtual void BeginPlay() override;
}; 
 