// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LSGameMode.generated.h"

class USoundBase;

UCLASS()
class LSPROJECT_API ALSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ALSGameMode();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundBase* StartMapBGM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundBase* DayBGM;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundBase* NightBGM;
};
