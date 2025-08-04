#include "Enemy/LSTestFence.h"

ALSTestFence::ALSTestFence()
{
	Scene=CreateDefaultSubobject<USceneComponent>("Scene");
	RootComponent = Scene;
	StaticMesh=CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(Scene);
	Health=100.0f;
}

float ALSTestFence::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Fence Take Damaged"))
	Health -= DamageAmount;
	//EnemyTodo : Delete() 호출 후 울타리 삭제
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
