#include "Enemy/LSEnemy.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ALSEnemy::ALSEnemy()
{
	AttackRange=30.0f;
	AttackDamage=30.0f;
	WalkSpeed=300.0f;
	StartVectorZ=30.0f;
}

void ALSEnemy::Attack()
{
	FVector StartLocation = GetActorLocation()+FVector(0.0f,0.0f,StartVectorZ);
	FVector EndLocation = StartLocation+ GetActorForwardVector() * AttackRange;
	TArray<FHitResult> Hits;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f);

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Attack Succeeded"))

	if (Anim)
	{
		Anim->StopAllMontages(0.5f);
		if (HitMontage)
		{
			UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] HitMontage Succeeded"))
			Anim->Montage_Play(HitMontage, 1.f);
		}
	}

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);
	
	DrawDebugLine(GetWorld(),StartLocation,EndLocation,FColor::Red,false, 3.0f);
	DrawDebugSphere(GetWorld(), StartLocation, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 2.0f);
	if (bHit)
	{
		for(const FHitResult& Hit : Hits)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->ActorHasTag("Player") || HitActor->ActorHasTag("Fence"))
				{
					UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Enemy Attack is Perfect Completed"));
					if (HitActor->ActorHasTag("Player"))
					{
						UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog]  is Player "));
					}
					UGameplayStatics::ApplyDamage(
						HitActor,
						AttackDamage,
						GetController(),
						this,
						UDamageType::StaticClass()
					);
				}
			}
		}
	}
}

float ALSEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Fence Take Damaged"))
	Health -= DamageAmount;
	if (Health<=0.0f)
	{
		Death();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ALSEnemy::Death()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (Anim)
	{
		Anim->StopAllMontages(0.5f);
		if (DeathMontage)
		{
			PlayAnimMontage(DeathMontage,1.0f);
		}
	}
	Destroy();
}

void ALSEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}
