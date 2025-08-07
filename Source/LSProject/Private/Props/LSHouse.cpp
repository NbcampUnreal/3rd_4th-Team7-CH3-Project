// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/LSHouse.h"
#include "Components/BoxComponent.h"

ALSHouse::ALSHouse()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	
	BoxCollision=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetupAttachment(StaticMeshComponent);
}

void ALSHouse::BeginPlay()
{
	Super::BeginPlay();
	
}


