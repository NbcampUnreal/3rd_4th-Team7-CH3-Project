#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LSBTTask_FindCloseFense.generated.h"

UCLASS()
class LSPROJECT_API ULSBTTask_FindCloseFense : public UBTTaskNode
{
	GENERATED_BODY()

	ULSBTTask_FindCloseFense();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;

	UPROPERTY()
	AAIController* CachedAIController;
	UPROPERTY()
	APawn* CachedAIPawn;
	
	FVector FindCloseFense(APawn* AIPawn) const;
};
