#pragma once

#include "CoreMinimal.h"
#include "Enemy/LSEnemy.h"
#include "LSBoss.generated.h"

UCLASS()
class LSPROJECT_API ALSBoss : public ALSEnemy
{
	GENERATED_BODY()
public:
	ALSBoss();

protected:
	virtual void Attack() override;
	
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; 

	UFUNCTION()
	void StartFirstMontage();
	
	void StartSecondPhase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Phase, meta = (AllowPrivateAccess = "true"))
	int32 BossPhase;
	
	bool IsBeatAble;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* StartMotionMontage = nullptr;
	UPROPERTY(EditAnywhere)
	UAnimMontage* NormalAttackMontage = nullptr;
};
