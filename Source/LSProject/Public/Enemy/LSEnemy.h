#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "LSEnemy.generated.h"

class USphereComponent;

UENUM(BlueprintType)
enum class ELSZombieType : uint8
{
	Normal,
	Fence,
	Big,
	Boss
};

UCLASS()
class LSPROJECT_API ALSEnemy : public ALSCharacterBase
{
	GENERATED_BODY()

public:
	ALSEnemy();
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void Death() override;

	void AddAbility(float AddHelth, float AddDamage);
	void HitAttack();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Overlap)
	USphereComponent* SphereComponent;

	FTimerHandle AttackTimerHandle;

	UFUNCTION()
	virtual void OnEnemyOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEnemyEndOverlap(
			UPrimitiveComponent* OverlappedComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Type)
	ELSZombieType ZombieType;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category=Coin)
	int32 EnemyCoin;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = Attack)
	float AttackRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float AttackDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float StartVectorZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float StartVectorY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float StartVectorX;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Sound")
	USoundBase* FenceSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* HitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Montage")
	UAnimMontage* DeathMontage;

	bool IsRotation=false;
	void SetDeltaRotation(float DeltaSeconds);

	UPROPERTY()
	AActor* Player=nullptr;
	
};
