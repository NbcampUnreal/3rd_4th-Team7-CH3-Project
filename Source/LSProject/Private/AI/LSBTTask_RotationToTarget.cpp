#include "AI/LSBTTask_RotationToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Chaos/CollectionPropertyFacade.h"
#include "Kismet/KismetMathLibrary.h"

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
	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;
	//EnemyTodo : 부드럽게 Rotation
	FVector TargetLocation = Blackboard->GetValueAsVector(TEXT("MoveToLocation"));
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), TargetLocation);

	FRotator Rotation= FMath::RInterpTo(AIPawn->GetActorRotation(), LookAtRotation, GetWorld()->GetTimeSeconds(), 0.5f);

	AIPawn->SetActorRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));
	
	return EBTNodeResult::Succeeded;
}
