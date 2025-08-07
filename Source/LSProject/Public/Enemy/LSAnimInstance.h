#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LSAnimInstance.generated.h"

UCLASS()
class LSPROJECT_API ULSAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	ULSAnimInstance();
protected:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "AIBoss")
	float CurrentVelocity;
};
