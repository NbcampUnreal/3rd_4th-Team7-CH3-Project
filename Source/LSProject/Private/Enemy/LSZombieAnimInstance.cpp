#include "Enemy/LSZombieAnimInstance.h"

ULSZombieAnimInstance::ULSZombieAnimInstance()
{
	CurrentVelocity=0.0f;
}

void ULSZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* Pawn = TryGetPawnOwner();
	if (Pawn == nullptr) return;
	
	FVector NowVelocity = Pawn->GetVelocity();
	NowVelocity.Z=0.0f;
	
	CurrentVelocity = NowVelocity.Size();
}