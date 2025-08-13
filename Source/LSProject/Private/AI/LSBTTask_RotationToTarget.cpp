#include "AI/LSBTTask_RotationToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

ULSBTTask_RotationToTarget::ULSBTTask_RotationToTarget()
{
	NodeName="Rotation To Target";
}

EBTNodeResult::Type ULSBTTask_RotationToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Controller = OwnerComp.GetAIOwner();
	if (!Controller) return EBTNodeResult::Failed;
	AIPawn = Controller->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	Blackboard = Controller->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}

void ULSBTTask_RotationToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!Controller || !AIPawn || !Blackboard) return;

	FVector TargetLocation = Blackboard->GetValueAsVector(TEXT("MoveToLocation"));
	FRotator GoToRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), TargetLocation);

	FRotator ActorRotation = AIPawn->GetActorRotation();

	FRotator Rotation= FMath::RInterpTo(ActorRotation, GoToRotation, DeltaSeconds, 5.f);
	
	AIPawn->SetActorRotation(FRotator(ActorRotation.Pitch, Rotation.Yaw, ActorRotation.Roll));

	if	(FMath::FindDeltaAngleDegrees(GoToRotation.Yaw,AIPawn->GetActorRotation().Yaw)<= 1.f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
