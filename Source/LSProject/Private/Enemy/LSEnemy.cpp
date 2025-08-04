#include "Enemy/LSEnemy.h"

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

	if (Anim)
	{
		Anim->StopAllMontages(0.5f);
		if (Montage)
		{
			Anim->Montage_Play(Montage, 1.f);
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

//EnemyTodo : TakeDamage 수정
// float ALSEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
// 	class AController* EventInstigator, AActor* DamageCauser)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Fence Take Damaged"))
// 	Health -= DamageAmount;
// 	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
// }

void ALSEnemy::Death()
{
	//EnemyTodo : Death() 내부에서 Destroy() 실행
	//EnemyTodo : AnimInstance의 Health 변수 Update > Death Motion 실행 > 아니면 그냥 몽타주 한 번 보여주고 죽기
	if (true)
	{
		EnemyDelete();
	}
}

void ALSEnemy::EnemyDelete()
{
	Destroy();
}
