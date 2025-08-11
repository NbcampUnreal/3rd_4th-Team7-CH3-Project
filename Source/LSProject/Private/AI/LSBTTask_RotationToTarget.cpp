#include "AI/LSBTTask_RotationToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

ULSBTTask_RotationToTarget::ULSBTTask_RotationToTarget()
{
	NodeName="Rotation To Target";
	bNotifyTick=true;
}

EBTNodeResult::Type ULSBTTask_RotationToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	
	// AAIController* AIController=OwnerComp.GetAIOwner();
	// if (!AIController) return EBTNodeResult::Failed;
	//
	// APawn* AIPawn = AIController->GetPawn();
	// if (!AIPawn) return EBTNodeResult::Failed;
	//
	// UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	// if (!Blackboard) return EBTNodeResult::Failed;
	//
	// //EnemyTodo : 부드럽게 Rotation
	// FVector TargetLocation = Blackboard->GetValueAsVector(TEXT("MoveToLocation"));
	// FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), TargetLocation);
	//
	// FRotator Rotation= FMath::RInterpTo(AIPawn->GetActorRotation(), LookAtRotation, GetWorld()->GetTimeSeconds(), 0.5f);
	//
	// AIPawn->SetActorRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));

	return EBTNodeResult::Succeeded;
}

void ULSBTTask_RotationToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return ;
	APawn* AIPawn = Controller->GetPawn();
	if (!AIPawn) return ;
	UBlackboardComponent* Blackboard = Controller->GetBlackboardComponent();
	if (!Blackboard) return ;
	FVector TargetLocation = Blackboard->GetValueAsVector(TEXT("MoveToLocation"));
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AIPawn->GetActorLocation(), TargetLocation);
	FRotator Rotation= FMath::RInterpTo(AIPawn->GetActorRotation(), LookAtRotation, GetWorld()->GetTimeSeconds(), 0.5f);

	AIPawn->SetActorRotation(FRotator(0.0f, Rotation.Yaw, 0.0f));

	if (FMath::IsNearlyEqual(LookAtRotation.Yaw , AIPawn->GetActorRotation().Yaw))
	{
	bNotifyTaskFinished = true;
		
	}
}
