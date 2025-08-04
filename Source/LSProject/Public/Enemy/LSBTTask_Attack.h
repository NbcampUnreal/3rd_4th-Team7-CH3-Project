#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LSBTTask_Attack.generated.h"

UCLASS()
class LSPROJECT_API ULSBTTask_Attack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	ULSBTTask_Attack();
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
