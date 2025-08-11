#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LSBTTask_RotationToTarget.generated.h"

UCLASS()
class LSPROJECT_API ULSBTTask_RotationToTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	ULSBTTask_RotationToTarget();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
