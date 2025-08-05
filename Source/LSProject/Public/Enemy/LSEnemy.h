#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "LSEnemy.generated.h"

UENUM(BlueprintType)
enum class ELSZombieType : uint8
{
	Normal	UMETA(DisplayName = "Normal"),
	Fence	UMETA(DisplayName = "Fence"),
	Big		UMETA(DisplayName = "Big")
};

UCLASS()
class LSPROJECT_API ALSEnemy : public ALSCharacterBase
{
	GENERATED_BODY()

public:
	ALSEnemy();
	virtual void Attack() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual void Death() override;

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Type)
	ELSZombieType ZombieType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float StartVectorZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* DeathMontage;
};
