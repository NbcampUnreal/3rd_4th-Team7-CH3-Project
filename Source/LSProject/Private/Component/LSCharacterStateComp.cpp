#include "Component/LSCharacterStateComp.h"
#include "Character/LSPlayerCharacter.h"

ULSCharacterStateComp::ULSCharacterStateComp()
{
	PrimaryComponentTick.bCanEverTick = false;

	CurrentState = ECharacterState::Idle;
}

void ULSCharacterStateComp::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ALSPlayerCharacter>(GetOwner());

	if (OwnerCharacter)
	{
		OwnerAnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
		if (OwnerAnimInstance)
		{
			OwnerAnimInstance->OnMontageEnded.AddDynamic(this, &ULSCharacterStateComp::OnMontageEnded);
		}
	}
}

void ULSCharacterStateComp::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OwnerAnimInstance)
	{
		OwnerAnimInstance->OnMontageEnded.RemoveDynamic(this, &ULSCharacterStateComp::OnMontageEnded);
	}
}

void ULSCharacterStateComp::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted) return;


	if (FMath::IsNearlyZero(OwnerCharacter->GetCurrentHealth()))
	{
		SetState(ECharacterState::Die);
	}
	else
	{
		SetState(ECharacterState::Idle);
	}

	UE_LOG(LogTemp, Warning, TEXT("Montage Ended"));
}

bool ULSCharacterStateComp::CanMove() const
{
	return CurrentState != ECharacterState::Die;
}

bool ULSCharacterStateComp::CanJump() const
{
	return CurrentState != ECharacterState::Die;
}

bool ULSCharacterStateComp::CanEquip() const
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

bool ULSCharacterStateComp::CanFire() const
{
	switch (CurrentState)
	{
	case ECharacterState::Equip:
		return false;
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
	case ECharacterState::Equip:
		return false;
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

bool ULSCharacterStateComp::CanDie() const
{
	switch (CurrentState)
	{
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

void ULSCharacterStateComp::SetState(ECharacterState NewState)
{
	if (CurrentState == NewState) return;

	CurrentState = NewState;
	UE_LOG(LogTemp, Warning, TEXT("Set State : %d"), CurrentState);
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
