#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "LSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
class ULSShopComp;
class ULSInventoryComp;
class ULSCharacterStateComp;
class ULSPlayerWeaponSystemComp;
class UUserWidget;
class UProgressBar;

UENUM(BlueprintType)
enum class ECurrentWeapon : uint8
{
	None UMETA(DisplayName = "None"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
};

UCLASS()
class LSPROJECT_API ALSPlayerCharacter : public ALSCharacterBase
{
	GENERATED_BODY()

public:
	ALSPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UFUNCTION(BlueprintPure, Category = "State")
	ECurrentWeapon GetCurrentWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetCurrentWeapon(ECurrentWeapon Weapon);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Store")
	TObjectPtr<ULSShopComp> ShopComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Store")
	TObjectPtr<ULSInventoryComp> InventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="StateComp")
	TObjectPtr<ULSCharacterStateComp> CharacterStateComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon") 
	TObjectPtr<ULSPlayerWeaponSystemComp> WeaponSystemComp;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const;
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthBar(float Current, float Max);

	UFUNCTION(BlueprintCallable, Category = "State")
	void Equip();
	
protected:
	virtual void BeginPlay() override;
	virtual void Attack() override;
	virtual void Death() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount,
	                         struct FDamageEvent const& DamageEvent,
	                         AController* EventInstigator,
	                         AActor* DamageCauser) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);
	UFUNCTION()
	void Reload(const FInputActionValue& Value);
	
	UFUNCTION()
	void OpenShopUI();
	UFUNCTION()
	void StartInvenUI();
	UFUNCTION()
	void EndInvenUI();

	UFUNCTION()
	void RestoreFence();
	
private:
	ECurrentWeapon CurrentWeapon;
	bool bCanFire;
	FTimerHandle FireTimerHandle;

	void ResetFireTimer();
	
	UPROPERTY()
	UAnimMontage* FireMontage;
	UPROPERTY()
	UAnimMontage* ReloadMontage;
	UPROPERTY()
	UAnimMontage* EquipMontage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> FireMontageCollection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> ReloadMontageCollection;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> EquipMontageCollection;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;
	UPROPERTY()
	UUserWidget* GameOverWidgetInstance = nullptr;
	UPROPERTY()
	UProgressBar* HUDHealthBar =nullptr;
	UPROPERTY()
	bool bShowGameOver = false;
	
}; 
 