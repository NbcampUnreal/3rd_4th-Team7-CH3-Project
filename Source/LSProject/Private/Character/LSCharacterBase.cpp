
#include "Character/LSCharacterBase.h"

ALSCharacterBase::ALSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	WalkSpeed = 600.0f;
	Health = 300.0f;
}

void ALSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALSCharacterBase::TakeDamage(float Damage)
{
}

void ALSCharacterBase::Attack()
{
}

void ALSCharacterBase::Death()
{
}

void ALSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

