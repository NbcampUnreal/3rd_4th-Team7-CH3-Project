// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "LSZombieAIController.generated.h"

UCLASS()
class LSPROJECT_API ALSZombieAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALSZombieAIController();

	UBlackboardComponent* GetBlackBoardComp() const;

	void StartBehaviorTree();
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Behavior)
	class UBehaviorTree* BehaviorTreeAsset;
	
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UAIPerceptionComponent* AIPerception;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UBlackboardComponent* BlackboardComp;
	
	
};
