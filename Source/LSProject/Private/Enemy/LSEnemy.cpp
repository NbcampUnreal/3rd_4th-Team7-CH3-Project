#include "Enemy/LSEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LSPlayerState.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

ALSEnemy::ALSEnemy()
{
	WalkSpeed=300.f;
	ZombieType=ELSZombieType::Normal;
	EnemyCoin=0;
	AttackRange=30.f;
	AttackDamage=30.f;
	StartVectorZ=30.f;
	StartVectorY=0.f;
	StartVectorX=30.f;
	HitMontage=nullptr;
	DeathMontage=nullptr;
	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetCapsuleComponent());
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALSEnemy::OnEnemyOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ALSEnemy::OnEnemyEndOverlap);
}

void ALSEnemy::Attack()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement(); //EnemyTodo : 리팩토링 요망
	}

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();

	if (Anim && HitMontage)
	{
		Anim->StopAllMontages(0.5f);
		Anim->Montage_Play(HitMontage, 1.f);
	}
}

float ALSEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Fence Take Damaged"))

	CurrentHealth -= DamageAmount;
	if (CurrentHealth<=0.0f)
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
	ALSPlayerState* GameState = Cast<ALSPlayerState>(GetPlayerState());
	if (GameState)
	{
		GameState->SetCoin(EnemyCoin);
	}
	Destroy();
}

void ALSEnemy::AddAbility(float AddHealth, float AddDamage)
{
	CurrentHealth+=AddHealth;
	AttackDamage+=AddDamage;
//	UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog]AddAbility SUCCEESS : AddHealth:%f, AddDamage:%f, NowHealth:%f, NowDamage:%f"),AddHealth,AddDamage,Health,AttackDamage)
}

void ALSEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (ZombieType == ELSZombieType::Fence)
	{
		//UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] ZombieType is Fence"));
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] FenceZom AIController"))
			UBlackboardComponent* Blackboard = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
			if (Blackboard)
			{
				UE_LOG(LogTemp,Warning,TEXT("[LSEnemyLog] BlackBoard Key Is Fence"))
				Blackboard->SetValueAsBool("IsFenceZom",true);
			}
		}
	}
}

void ALSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsRotation == true)
	{
		SetDeltaRotation(DeltaTime);
	}
}

//montage의 notify에서 실행됨
void ALSEnemy::HitAttack()
{
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.0f);
	FVector LocalOffset(StartVectorX, StartVectorY, StartVectorZ);
	FVector StartLocation = GetActorTransform().TransformPosition(LocalOffset);
	FVector EndLocation = StartLocation+ GetActorForwardVector() * AttackRange;
	TArray<FHitResult> Hits;

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Pawn,
		CollisionShape
	);

	DrawDebugSphere(GetWorld(), StartLocation, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 0.5f);
	if (bHit)
	{
		for(const FHitResult& Hit : Hits)
		{
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->ActorHasTag("Player"))
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
				else if (HitActor->ActorHasTag("Fence"))
				{
					//사운드 재생
					if (FenceSound)
					{
						UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Enemy Attack is Perfect Completed"));					
						UGameplayStatics::PlaySoundAtLocation(this, FenceSound, GetActorLocation());
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

void ALSEnemy::OnEnemyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep,	const FHitResult& SweepResult)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!OtherActor || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Overlap Is Failed"))
	}
	if (OtherActor==Player)
	{
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &ALSEnemy::Attack, 1.7f, true, 0.0f);
		IsRotation = true;
	}
}

void ALSEnemy::OnEnemyEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!OtherActor || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Overlap Is Failed"))
	}
	if (OtherActor==Player)
	{
		GetWorldTimerManager().ClearTimer(AttackTimerHandle);
	}
}

void ALSEnemy::SetDeltaRotation(float DeltaSeconds)
{
	if (!Player)
	{
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	}
	FRotator NowRotation =  GetActorRotation();
	FRotator LookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation());

	FRotator GoRotation = FMath::RInterpTo(NowRotation, LookRotation, DeltaSeconds, 3.f);

	SetActorRotation(FRotator(0.f, GoRotation.Yaw, 0.f));

	if	(FMath::Abs(FMath::FindDeltaAngleDegrees(LookRotation.Yaw,GetActorRotation().Yaw))<= 1.f)
	{
		IsRotation=false;
	}
}
