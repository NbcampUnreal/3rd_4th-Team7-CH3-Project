// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LSZombieAIController.generated.h"

UCLASS()
class LSPROJECT_API ALSZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALSZombieAIController();

	FORCEINLINE
	UBlackboardComponent* GetBlackBoardComp() const
	{
		return BlackboardComp;
	}

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Behavior)
	class UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BossBlackboardComp;
	
	
};
