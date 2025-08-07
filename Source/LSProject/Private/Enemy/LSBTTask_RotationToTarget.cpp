#include "Enemy/LSBTTask_RotationToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Chaos/CollectionPropertyFacade.h"

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
	FVector PlayerLocation = Blackboard->GetValueAsVector(TEXT("PlayerLocation"));
	if (PlayerLocation==FVector::ZeroVector)
	{
		FVector FenceLocation = Blackboard->GetValueAsVector(TEXT("ClosestFenceLocation"));
		if (FenceLocation==FVector::ZeroVector) return EBTNodeResult::Failed;
		AIPawn->SetActorRotation(FRotator(FenceLocation.X, 0.0f, 0.0f));
	}
	SetRotationToTarget(PlayerLocation);
	return EBTNodeResult::Succeeded;
}

void ULSBTTask_RotationToTarget::SetRotationToTarget(FVector GoVector)
{
	//NowRotation
	//GoVector
	// FRotator CurrentRotation = GetActorRotation();
	// FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	//
	// float InterpSpeed = 5.0f;
	// FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, GetWorld()->GetDeltaSeconds(), InterpSpeed);
	// SetActorRotation(NewRotation);
	
}
