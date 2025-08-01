
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
	Pistol		UMETA(DisplayName = "Pistol"),
	Rifle		UMETA(DisplayName = "Rifle"),
	Shotgun		UMETA(DisplayName = "Shotgun")
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

protected:
	virtual void TakeDamage(float Damage) override;
	virtual void Attack() override;
	virtual void Death() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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

private:
	ECurrentWeapon CurrentWeapon;
	
};
