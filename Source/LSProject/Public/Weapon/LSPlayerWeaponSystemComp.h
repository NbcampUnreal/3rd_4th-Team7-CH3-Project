#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSPlayerWeaponSystemComp.generated.h"

class ALSPlayerCharacter;
class ALSWeaponBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LSPROJECT_API ULSPlayerWeaponSystemComp : public UActorComponent
{
	GENERATED_BODY()

public:
	ULSPlayerWeaponSystemComp();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ALSPlayerCharacter> OwnerCharacter;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<TSubclassOf<ALSWeaponBase>> WeaponClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<ALSWeaponBase> CurrentWeapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool IsArmedWeapon;

	UFUNCTION()
	void EquipWeapon();
	UFUNCTION()
	void UnEquipWeapon();
};
