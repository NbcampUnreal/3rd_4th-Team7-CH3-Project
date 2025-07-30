// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/LSCharacterBase.h"

// Sets default values
ALSCharacterBase::ALSCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALSCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALSCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

