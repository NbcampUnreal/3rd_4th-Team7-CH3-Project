// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSInventoryComp.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LSPROJECT_API ULSInventoryComp : public UActorComponent
{
	GENERATED_BODY()

public:

	ULSInventoryComp();
protected:
	virtual void BeginPlay() override;

public:
	void AddToInven(const FName& Input,int32 Amount);

	//추후 삭제
	UFUNCTION(Blueprintable)
	void ShowInven();
private:
	UPROPERTY()
	TMap<FName,int32> MyItems;
};
