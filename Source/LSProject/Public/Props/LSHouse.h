// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSHouse.generated.h"

class UBoxComponent;

UCLASS()
class LSPROJECT_API ALSHouse : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSHouse();

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Settings")
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Settings")	
	TObjectPtr<UBoxComponent> BoxCollision;
	
protected:
	virtual void BeginPlay() override;

};
