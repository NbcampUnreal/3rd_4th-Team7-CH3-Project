#include "Component/LSCharacterStateComp.h"
#include "Character/LSPlayerCharacter.h"

ULSCharacterStateComp::ULSCharacterStateComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentState = ECharacterState::Idle;
	CurrentMontageDuration = 0.0f;
	MontageStartTime = 0.0f;
}

void ULSCharacterStateComp::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ALSPlayerCharacter>(GetOwner());
}


void ULSCharacterStateComp::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState != ECharacterState::Idle)
	{
		float ElapsedTime = GetWorld()->GetTimeSeconds() - MontageStartTime;
		if (ElapsedTime >= CurrentMontageDuration)
		{
			SetState(ECharacterState::Idle);
		}
	}
}

bool ULSCharacterStateComp::CanMove() const
{
	return CurrentState != ECharacterState::Die;
}

bool ULSCharacterStateComp::CanJump() const
{
	return CurrentState != ECharacterState::Die;
}

bool ULSCharacterStateComp::CanFire() const
{
	switch (CurrentState)
	{
	case ECharacterState::Reload:
		return false;
	case ECharacterState::Die:
		return false;
	default:
		return true;
	}
}

bool ULSCharacterStateComp::CanReload() const
{
	switch (CurrentState)
	{
	case ECharacterState::Fire:
		return false;
	case ECharacterState::Reload:
		return false;
	case ECharacterState::Die:
		return false;
	default:
		return true;
	}
}

ECharacterState ULSCharacterStateComp::GetCurrentState() const
{
	return CurrentState;
}

void ULSCharacterStateComp::SetCurrentMontageDuration(float Duration)
{
	CurrentMontageDuration = Duration;
	MontageStartTime = GetWorld()->GetTimeSeconds();
}

void ULSCharacterStateComp::SetState(ECharacterState NewState)
{
	if (CurrentState == NewState) return;

	CurrentState = NewState;
}

void ULSCharacterStateComp::StopCurrentMontage()
{
	if (!OwnerCharacter) return;

	if (UAnimInstance* AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		if (AnimInstance->IsAnyMontagePlaying())
		{
			AnimInstance->Montage_Stop(0.25f);
		}
	}
}
