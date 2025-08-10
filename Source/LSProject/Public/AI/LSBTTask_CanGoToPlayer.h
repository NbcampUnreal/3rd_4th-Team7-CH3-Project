#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "LSBTTask_CanGoToPlayer.generated.h"

UCLASS()
class LSPROJECT_API ULSBTTask_CanGoToPlayer : public UBTTaskNode
{
	GENERATED_BODY()
public:
	ULSBTTask_CanGoToPlayer();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory) override;
};
