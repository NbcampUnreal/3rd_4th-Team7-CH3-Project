#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LSBTTask_RotationToFence.generated.h"

UCLASS()
class LSPROJECT_API ULSBTTask_RotationToFence : public UBTTaskNode
{
	GENERATED_BODY()
	ULSBTTask_RotationToFence();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void RotationToTarget();
	
};
