#include "Props/LSTestFence.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ALSTestFence::ALSTestFence()
{
	Scene=CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = Scene;
	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(Scene);
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	BoxCollision->SetupAttachment(Scene);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALSTestFence::OnFenceOverlap);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALSTestFence::OnFenceEndOverlap);
	Health=100.0f;
}

float ALSTestFence::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Fence Take Damaged"))
	Health -= DamageAmount;
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Now Fence Health : %f"), Health)
	if (Health<=0.0f)
	{
		Destroy();
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ALSTestFence::OnFenceOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!OtherActor || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Overlap Is Failed"))
	}
	if (OtherActor==Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSFence] Overlap Succeess"))
	}
}

void ALSTestFence::OnFenceEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	if (!OtherActor || !Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Overlap Is Failed"))
	}
	if (OtherActor==Player)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[LSFence] Overlap End"))
	}
	
}
