#include "Enemy/LSEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LSPlayerState.h"

ALSEnemy::ALSEnemy()
{
	WalkSpeed=300.0f;
	ZombieType=ELSZombieType::Normal;
	EnemyCoin=0;
	AttackRange=30.0f;
	AttackDamage=30.0f;
	StartVectorZ=30.0f;
	HitMontage=nullptr;
	DeathMontage=nullptr;
}

void ALSEnemy::Attack()
{
	FVector StartLocation = GetActorLocation()+FVector(0.0f,0.0f,StartVectorZ);
	FVector EndLocation = StartLocation+ GetActorForwardVector() * AttackRange;
	TArray<FHitResult> Hits;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f);

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (Anim && HitMontage)
	{
		Anim->StopAllMontages(0.5f);
		Anim->Montage_Play(HitMontage, 1.f);
	}

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);

	//EnemyTodo : SphereTrace랑 LineTrace랑 둘 다 Hit 합치기
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

	// Health -= DamageAmount;
	// if (Health<=0.0f)
	// {
	// 	Death();
	// }
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
	ALSPlayerState* GameState = Cast<ALSPlayerState>(GetPlayerState());
	if (GameState)
	{
		GameState->SetCoin(EnemyCoin);
	}
	Destroy();
}

void ALSEnemy::AddAbility(float AddHealth, float AddDamage)
{
//	Health+=AddHealth;
	AttackDamage+=AddDamage;
//	UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog]AddAbility SUCCEESS : AddHealth:%f, AddDamage:%f, NowHealth:%f, NowDamage:%f"),AddHealth,AddDamage,Health,AttackDamage)
}

void ALSEnemy::BeginPlay()
{
	Super::BeginPlay();
	FString EnumZombieType = UEnum::GetValueAsString(ZombieType);
	UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] ZombieType : %s"), *EnumZombieType)
	
	if (ZombieType == ELSZombieType::Fence)
	{
		//UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] ZombieType is Fence"));
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			//UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] FenceZom AIController"));
			UBlackboardComponent* Blackboard = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
			if (Blackboard)
			{
				//UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] BlackBoard Key Is Fence"));
				Blackboard->SetValueAsBool("IsFenceZom",true);
			}
		}
	}
	
	//UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] Zombie BeginPlay Is Finished"))
}
