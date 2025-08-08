#include "Enemy/LSBoss.h"

#include "Kismet/GameplayStatics.h"

ALSBoss::ALSBoss()
{
	AttackDamage = 30.0f;
	//Health = 300.0f;
	BossPhase=1;
	IsBeatAble=false;
}

void ALSBoss::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle FirstTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		FirstTimerHandle,
		this,
		&ALSBoss::StartFirstMontage,
		2.f,
		false
		);
}

void ALSBoss::StartFirstMontage()
{
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (Anim && StartMotionMontage)
	{
		Anim->StopAllMontages(0.5f);
		Anim->Montage_Play(StartMotionMontage,1.f);
	}
}

float ALSBoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (!IsBeatAble) return 0.f;
	//Health-=DamageAmount;
	// if (Health <= 0.0f)
	// {
	// 	if (BossPhase==1)
	// 	{
	// 		StartSecondPhase();
	// 	}
	// 	else if (BossPhase==2)
	// 	{
	// 		Death();
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Boss Phase Is ERROR"))
	// 	}
	// }
	return DamageAmount;
}

void ALSBoss::StartSecondPhase()
{
	BossPhase=2;
	//Health=500.0f;
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Boss 2 PHASE START"));
}

void ALSBoss::Attack()
{
	FVector StartLocation = GetActorLocation()+FVector(0.0f,0.0f,StartVectorZ);
	FVector EndLocation = StartLocation+GetActorForwardVector()*AttackRange;
	TArray<FHitResult> HitResults;
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f);

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (Anim && NormalAttackMontage)
	{
		UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] AttackMontage IS SUCCEESS"))
		Anim->StopAllMontages(0.5f);
		Anim->Montage_Play(NormalAttackMontage , 1.f);
	}

	bool bHit = GetWorld()->SweepMultiByChannel(
		HitResults,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);
	DrawDebugSphere(GetWorld(), StartLocation, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 2.0f);
	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->ActorHasTag("Player") || HitActor->ActorHasTag("Fence"))
				{
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

