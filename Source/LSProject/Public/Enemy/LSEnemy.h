#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "LSEnemy.generated.h"

UCLASS()
class LSPROJECT_API ALSEnemy : public ALSCharacterBase
{
	GENERATED_BODY()

public:
	ALSEnemy();
	void Attack() override;
	
	// virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	// class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void Death() override;
	virtual void EnemyDelete();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float StartVectorZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* Montage;
};
