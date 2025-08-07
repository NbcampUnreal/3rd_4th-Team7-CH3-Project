#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSCharacterStateComp.generated.h"

class ALSPlayerCharacter;

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle,
	Fire,
	Reload,
	Die
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LSPROJECT_API ULSCharacterStateComp : public UActorComponent
{
	GENERATED_BODY()

public:
	ULSCharacterStateComp();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:

	UPROPERTY()
	TObjectPtr<ALSPlayerCharacter> OwnerCharacter;
	UPROPERTY()
	TObjectPtr<UAnimInstance> OwnerAnimInstance;

	ECharacterState CurrentState;

public:
	bool CanMove() const;
	bool CanJump() const;
	bool CanFire() const;
	bool CanReload() const;
	ECharacterState GetCurrentState() const;
	void SetState(ECharacterState NewState);

private:
	void StopCurrentMontage();
};
