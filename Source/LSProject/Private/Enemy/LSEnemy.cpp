#include "Enemy/LSEnemy.h"

#include "Kismet/GameplayStatics.h"

ALSEnemy::ALSEnemy()
{
	AttackRange=30.0f;
	AttackDamage=30.0f;
	WalkSpeed=300.0f;
	
	
}

void ALSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	
	
}

void ALSEnemy::Attack()
{
	FVector StartLocation = GetActorLocation();
	FVector EndLocation = StartLocation+ GetActorForwardVector() * AttackRange;
	TArray<FHitResult> Hits;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f);

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);

	if (bHit)
	{
		for(const FHitResult& Hit : Hits)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->ActorHasTag("Player") || HitActor->ActorHasTag("Fence"))
				{
					DrawDebugSphere(GetWorld(), StartLocation, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 2.0f);
					DrawDebugSphere(GetWorld(), EndLocation, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 2.0f);
					
					UGameplayStatics::ApplyDamage(
						HitActor,
						AttackDamage,
						GetController(),
						this,
						UDamageType::StaticClass()
					);

					DrawDebugSphere(GetWorld(),Hit.ImpactPoint, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 2.0f);
				}
			}
		}
	}
}

void ALSEnemy::Death()
{
	//EnemyTodo : characterBase의 health<=0 ? EnemyDelete()
	if (true)
	{
		EnemyDelete();
	}
}

void ALSEnemy::TakeDamage(float Damage)
{
	Super::TakeDamage(Damage);

	Health -= Damage;
	if (Health <= 0)
	{
		Death();
	}
}

void ALSEnemy::EnemyDelete()
{
	Destroy();
}
