#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LSZombieAnimInstance.generated.h"

UCLASS()
class LSPROJECT_API ULSZombieAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULSZombieAnimInstance();

	void SetCurrentVelocity();

	void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	float CurrentVelocity;
};
