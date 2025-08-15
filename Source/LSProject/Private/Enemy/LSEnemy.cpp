#include "Enemy/LSEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LSPlayerState.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Game/LSGameState.h"
#include "Kismet/KismetMathLibrary.h"

ALSEnemy::ALSEnemy()
{
	WalkSpeed=300.f;
	ZombieType=ELSZombieType::Normal;
	EnemyCoin=0;
	AttackDamage=30.f;
	StartVectorZ=30.f;
	StartVectorY=0.f;
	StartVectorX=30.f;
	IsHited=false;
	HitMontage=nullptr;
	TakeDamageMontage=nullptr;
	DeathMontage=nullptr;
	CameraComp=nullptr;
	
	SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetCapsuleComponent());
	
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ALSEnemy::OnEnemyOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ALSEnemy::OnEnemyEndOverlap);
	
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(GetCapsuleComponent());
}

void ALSEnemy::Attack()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement(); //EnemyTodo : 리팩토링 요망
	}

	if (!CachedAnim)
	{
		CachedAnim=GetMesh()->GetAnimInstance();
	}
	if (CachedAnim && HitMontage)
	{
		CachedAnim->StopAllMontages(0.5f);
		CachedAnim->Montage_Play(HitMontage, 1.f);
	}
}

float ALSEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	CurrentHealth -= DamageAmount;
	UpdateCurrentHealth();
	if (CurrentHealth<=0.0f)
	{
		Death();
		return DamageAmount;
	}
	
	if (!CachedAnim)
	{
		CachedAnim = GetMesh()->GetAnimInstance();
	}
	if (CachedAnim)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			AIController->StopMovement(); //EnemyTodo : 리팩토링 요망
		}
		CachedAnim->StopAllMontages(0.f);
		if (TakeDamageMontage)
		{
			CachedAnim->Montage_Play(TakeDamageMontage,1.0f);
		}
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ALSEnemy::Death()
{
	if (IsDeath) return;
	IsDeath=true;
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement(); //EnemyTodo : 리팩토링 요망
	}
	if (!CachedAnim)
	{
		CachedAnim=GetMesh()->GetAnimInstance();
	}
	if (CachedAnim)
	{
		CachedAnim->StopAllMontages(0.5f);
		if (DeathMontage)
		{
			CachedAnim->Montage_Play(DeathMontage,1.0f);
		}
	}

	ALSPlayerState* GameState = Cast<ALSPlayerState>(GetPlayerState());
	if (GameState)
	{
		GameState->SetCoin(EnemyCoin);
	}

	if (ALSGameState* GS = GetWorld() ? GetWorld()->GetGameState<ALSGameState>() : nullptr)
	{
		GS->OnEnemyKilled();
	}

	//TimerHandle로 딜레이 걸고 3초 뒤 Destroy
	GetWorld()->GetTimerManager().SetTimer(
		DeathTimerHandle,
		this,
		&ALSEnemy::Delete,
		1.f,
		false
	);
}

void ALSEnemy::AddAbility(float AddHealth, float AddDamage)
{
	CurrentHealth+=AddHealth;
	AttackDamage+=AddDamage;
}

void ALSEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (ZombieType == ELSZombieType::Fence)
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		if (AIController)
		{
			UBlackboardComponent* Blackboard = Cast<UBlackboardComponent>(AIController->GetBlackboardComponent());
			if (Blackboard)
			{
				Blackboard->SetValueAsBool("IsFenceZom",true);
			}
		}
	}

	GetWorld()->GetTimerManager().SetTimer(
			HealthTimerHandle,
			this,
			&ALSEnemy::HealthRotation,
			0.1f,
			true
	);
}

//회전 : 잘 안 됨...
void ALSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsRotation == true)
	{
		SetDeltaRotation(DeltaTime);
	}
}

//montage의 notify에서 호출됨
void ALSEnemy::HitAttack()
{
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(50.f);
	FVector LocalOffset(StartVectorX, StartVectorY, StartVectorZ);
	FVector StartLocation = GetActorTransform().TransformPosition(LocalOffset);
	TArray<FHitResult> Hits;

	bool bHit = GetWorld()->SweepMultiByChannel(
		Hits,
		StartLocation,
		StartLocation,
		FQuat::Identity,
		ECC_Visibility,
		CollisionShape
	);

	IsHited=false;
	DrawDebugSphere(GetWorld(), StartLocation, CollisionShape.GetSphereRadius(), 16, FColor::Red,false, 0.5f);

	if (bHit)
	{
		for(const FHitResult& Hit : Hits)
		{
			if (IsHited) return;
			if (AActor* HitActor = Hit.GetActor())
			{
				if (HitActor->ActorHasTag("Player"))
				{
					IsHited = true;
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
					IsHited = true;
					//사운드 재생
					if (FenceSound)
					{
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
                              UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ALSEnemy::UpdateCurrentHealth()
{
	if (!WidgetComponent) return;
	if (!UserWidget)
	{
		UserWidget = WidgetComponent->GetWidget();
	}
	if (UserWidget)
	{
		if(UProgressBar* ProgressBar = Cast<UProgressBar>(UserWidget->GetWidgetFromName(TEXT("FenceHealthBar"))))
		{
			float HealthPercent = CurrentHealth/MaxHealth;
			ProgressBar->SetPercent(HealthPercent);
		}
	}
}

//잘 안 됨...
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

void ALSEnemy::HealthRotation()
{
	if (!WidgetComponent) return;
	if (!Player)
	{
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
		CameraComp = Player->GetComponentByClass<UCameraComponent>();
	}
	if (!CameraComp)
	{
		CameraComp = Player->GetComponentByClass<UCameraComponent>();
	}
	FVector PlayerLocation = CameraComp->GetComponentLocation();
	FVector WidgetLocation = WidgetComponent->GetComponentLocation();
	
	FVector Direction = (PlayerLocation - WidgetLocation).GetSafeNormal();
	
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	
	WidgetComponent->SetWorldRotation(LookAtRotation);
}

void ALSEnemy::Delete()
{
	if (HealthTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(HealthTimerHandle);
		HealthTimerHandle.Invalidate();
	}
	if (DeathTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DeathTimerHandle);
		DeathTimerHandle.Invalidate();
	}
	Destroy();
}
