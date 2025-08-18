#include "AI/LSBTTask_RotationToFence.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


ULSBTTask_RotationToFence::ULSBTTask_RotationToFence()
{
	NodeName="Rotation To Fence";
}

EBTNodeResult::Type ULSBTTask_RotationToFence::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	AActor* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	FVector PawnLocation = AIPawn->GetActorLocation();
	FVector FenceVector = Blackboard->GetValueAsVector(TEXT("MoveToLocation"));

	//FenceVector로 회전시키기
	FVector Direction = (FenceVector - PawnLocation).GetSafeNormal();
	
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	
	AIPawn->SetActorRotation(FRotator(AIPawn->GetActorRotation().Pitch ,LookAtRotation.Yaw, AIPawn->GetActorRotation().Roll));
	
	return EBTNodeResult::Succeeded;
}

void ULSBTTask_RotationToFence::RotationToTarget()
{
	
}
