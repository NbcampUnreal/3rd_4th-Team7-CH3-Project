#include "Enemy/LSAnimInstance.h"

ULSAnimInstance::ULSAnimInstance()
{
	CurrentVelocity=0.0f;
}

void ULSAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	APawn* Pawn = TryGetPawnOwner();
	if (!Pawn) return;
	FVector NowVelocity = Pawn->GetVelocity();
	NowVelocity.Z=0.0f;

	CurrentVelocity=NowVelocity.Size();
}
