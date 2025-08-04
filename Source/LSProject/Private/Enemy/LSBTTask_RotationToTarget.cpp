#include "Enemy/LSBTTask_RotationToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

ULSBTTask_RotationToTarget::ULSBTTask_RotationToTarget()
{
	NodeName="Rotation To Target";
}

EBTNodeResult::Type ULSBTTask_RotationToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController=OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	
	
	return EBTNodeResult::Succeeded;
}

void ULSBTTask_RotationToTarget::SetRotationToTarget()
{
	
}
