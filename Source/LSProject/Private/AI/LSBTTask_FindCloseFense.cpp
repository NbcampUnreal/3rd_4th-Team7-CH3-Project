#include "AI/LSBTTask_FindCloseFense.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Props/LSTestFence.h"
#include "Kismet/GameplayStatics.h"

ULSBTTask_FindCloseFense::ULSBTTask_FindCloseFense()
{
	NodeName = "FindCloseFense";
	CachedAIController=nullptr;
	CachedAIPawn=nullptr;
}

EBTNodeResult::Type ULSBTTask_FindCloseFense::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	
	if (!CachedAIController)
	{
		CachedAIController = Comp.GetAIOwner();
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BTTask : Controller is Not Found"))
	}
	
	if (!CachedAIPawn)
	{
		CachedAIPawn = CachedAIController->GetPawn();
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BTTask : AIPawn is Not Found"))
	}
	
	FVector CloseFenceLocation =  FindCloseFense(CachedAIPawn);
	if (CloseFenceLocation==FVector::ZeroVector)
	{
		//UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BTTask : FindCloseFence is Not Found"))
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
