#include "Component/LSMontagePlayComp.h"
#include "Character/LSPlayerCharacter.h"

ULSMontagePlayComp::ULSMontagePlayComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurrentState = ECharacterState::Idle;
	CurrentMontageDuration = 0.0f;
	MontageStartTime = 0.0f;
}

void ULSMontagePlayComp::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ALSPlayerCharacter>(GetOwner());
}


void ULSMontagePlayComp::TickComponent(float DeltaTime, ELevelTick TickType,
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

bool ULSMontagePlayComp::CanMove() const
{
	return CurrentState != ECharacterState::Die;
}

bool ULSMontagePlayComp::CanJump() const
{
	return CurrentState != ECharacterState::Die;
}

bool ULSMontagePlayComp::CanFire() const
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

bool ULSMontagePlayComp::CanReload() const
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

ECharacterState ULSMontagePlayComp::GetCurrentState() const
{
	return CurrentState;
}

void ULSMontagePlayComp::SetState(ECharacterState NewState)
{
	if (CurrentState == NewState) return;

	CurrentState = NewState;

	// todo: 정말 set이 이게 끝인가?
}

void ULSMontagePlayComp::StopCurrentMontage()
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
