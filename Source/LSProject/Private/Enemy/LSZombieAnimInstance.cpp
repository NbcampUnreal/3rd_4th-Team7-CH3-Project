#include "Enemy/LSZombieAnimInstance.h"

#include "Enemy/LSEnemy.h"

ULSZombieAnimInstance::ULSZombieAnimInstance()
{
}

void ULSZombieAnimInstance::SetCurrentVelocity()
{
	
	
}

void ULSZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	//todo : 액터의 속도를 반환
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn == nullptr) return;
	
	FVector NowVelocity = Pawn->GetVelocity();
	NowVelocity.Z=0.0f;
	
	CurrentVelocity = NowVelocity.Size();
}