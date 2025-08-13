#include "Enemy/LSBoss.h"

#include "AIController.h"

ALSBoss::ALSBoss()
{
	AttackDamage = 30.0f;
	CurrentHealth = 300.0f;
	BossPhase=1;
	IsBeatAble=false;
}

void ALSBoss::Attack()
{
	Super::Attack();
	
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
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController)
	{
		AIController->StopMovement(); //EnemyTodo : 리팩토링 요망
	}
	
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
	CurrentHealth-=DamageAmount;
	
	 if (CurrentHealth <= 0.0f)
	 {
	 	if (BossPhase==1)
	 	{
	 		StartSecondPhase();
	 	}
	 	else if (BossPhase==2)
	 	{
	 		Death();
	 	}
	 	else
	 	{
	 		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Boss Phase Is ERROR"))
	 	}
	 }
	return DamageAmount;
}

void ALSBoss::StartSecondPhase()
{
	BossPhase=2;
	//Health=500.0f;
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Boss 2 PHASE START"));
}

