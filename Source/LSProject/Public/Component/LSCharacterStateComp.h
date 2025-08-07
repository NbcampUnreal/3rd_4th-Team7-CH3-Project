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
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	ECharacterState CurrentState;

	UPROPERTY()
	TObjectPtr<ALSPlayerCharacter> OwnerCharacter;

	float CurrentMontageDuration;
	float MontageStartTime;

public:
	bool CanMove() const;
	bool CanJump() const;
	bool CanFire() const;
	bool CanReload() const;
	ECharacterState GetCurrentState() const;

	void SetCurrentMontageDuration(float Duration);

private:
	void SetState(ECharacterState NewState);
	void StopCurrentMontage();
};
