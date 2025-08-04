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

protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "AI")
	float CurrentVelocity;
};
