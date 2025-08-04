#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "LSPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;

UENUM(BlueprintType)
enum class ECurrentWeapon : uint8
{
	None UMETA(DisplayName = "None"),
	Pistol UMETA(DisplayName = "Pistol"),
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun")
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

	UFUNCTION(Blueprintpure, Category = "State")
	ECurrentWeapon GetCurrentWeapon() const;
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetCurrentWeapon(ECurrentWeapon Weapon);

protected:
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
	void Fire(const FInputActionValue& Value);

private:
	ECurrentWeapon CurrentWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* FireMontage;
};
