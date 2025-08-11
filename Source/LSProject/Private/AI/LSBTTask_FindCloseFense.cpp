#include "AI/LSBTTask_FindCloseFense.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Props/LSTestFence.h"
#include "Kismet/GameplayStatics.h"

ULSBTTask_FindCloseFense::ULSBTTask_FindCloseFense()
{
	NodeName = "FindCloseFense";
}

EBTNodeResult::Type ULSBTTask_FindCloseFense::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AAIController* AIController = Comp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Controller is Not Found in FindCloseFence"))
	}
	
	APawn* AIPawn = AIController->GetPawn();
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] AIPawn Name : %s"), *AIPawn->GetName())
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] AIPawn is Not Found in FindCloseFence"))
	}
	
	FVector CloseFenceLocation =  FindCloseFense(AIPawn);
	if (CloseFenceLocation==FVector::ZeroVector)
	{
		return EBTNodeResult::Failed;
	}
	Comp.GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"),CloseFenceLocation);
	return EBTNodeResult::Succeeded;
}

FVector ULSBTTask_FindCloseFense::FindCloseFense(APawn* AIPawn) const
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALSTestFence::StaticClass(), AllActors);
	
	AActor* NearestFence = nullptr;
	float MinDistance = FLT_MAX;

	for (AActor* Actor : AllActors)
	{
		if (ALSTestFence* NowFence = Cast<ALSTestFence>(Actor))
		{
			float NowDistance = FVector::DistSquared(NowFence->GetActorLocation(), AIPawn->GetActorLocation());
			
			if (NowDistance < MinDistance)
			{
				MinDistance=NowDistance;
				NearestFence=NowFence;
			}
		}
	}
	if (!NearestFence)
	{
		return FVector::ZeroVector;
	}
	return NearestFence->GetActorLocation();
}
