// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/LSHouse.h"

#include "Character/LSPlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Game/LSGameState.h"

ALSHouse::ALSHouse()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	
	BoxCollision=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMeshComponent);
	//BoxCollision->ComponentTags.Add(FName("Door"));
	//BoxCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

void ALSHouse::BeginPlay()
{
	Super::BeginPlay();

	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALSHouse::OnOverlapBegin);
	BoxCollision->OnComponentEndOverlap.AddDynamic(this, &ALSHouse::OnOverlapEnd);
}

void ALSHouse::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ALSPlayerCharacter* Character=Cast<ALSPlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp,Warning,TEXT("Overlap begin with Door"));
		if (ALSGameState* GS=GetWorld()->GetGameState<ALSGameState>())
		{
			GS->SetDoorOverlapped(true);
		}
	}
}

void ALSHouse::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (ALSPlayerCharacter* Character=Cast<ALSPlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp,Warning,TEXT("Overlap end with Door"));
		if (ALSGameState* GS=GetWorld()->GetGameState<ALSGameState>())
		{
			GS->SetDoorOverlapped(false);
		}
	}
}



