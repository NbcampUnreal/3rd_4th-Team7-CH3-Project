#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LSBTTask_FindCloseFense.generated.h"

UCLASS()
class LSPROJECT_API ULSBTTask_FindCloseFense : public UBTTaskNode
{
	GENERATED_BODY()

	ULSBTTask_FindCloseFense();

	FVector FindCloseFense(APawn* AIPawn);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
