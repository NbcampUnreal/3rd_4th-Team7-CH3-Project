#include "Enemy/LSBTTask_FindCloseFense.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/LSTestFence.h"
#include "Kismet/GameplayStatics.h"

ULSBTTask_FindCloseFense::ULSBTTask_FindCloseFense()
{
	NodeName = "FindCloseFense";
	ClosestFenceLocationKey.AddVectorFilter(this,GET_MEMBER_NAME_CHECKED(ULSBTTask_FindCloseFense,ClosestFenceLocationKey));
}

EBTNodeResult::Type ULSBTTask_FindCloseFense::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AAIController* AIController = Comp.GetAIOwner(); //todo : 위치 변경
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Controller is Not Found in FindCloseFence"))
	}
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] AIPawn is Not Found in FindCloseFence"))
	}
	
	FVector CloseFenceLocation =  FindCloseFense(Comp, AIPawn);
	if (CloseFenceLocation==FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] GetCloseFenceVectorIsNoFind"))
		return EBTNodeResult::Failed;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Get Close Fence Vector Is SUCCEEDED"))
		Comp.GetBlackboardComponent()->SetValueAsVector(ClosestFenceLocationKey.SelectedKeyName,CloseFenceLocation);
		return EBTNodeResult::Succeeded;
	}
}

FVector ULSBTTask_FindCloseFense::FindCloseFense(UBehaviorTreeComponent& Comp, APawn* AIPawn)
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALSTestFence::StaticClass(), AllActors);


	
	AActor* NearestFence = nullptr;
	float MinDistance = FLT_MAX;

	for (AActor* Actor : AllActors)
	{
		if (ALSTestFence* NowFence = Cast<ALSTestFence>(Actor))
		{
			//EnemyTodo : 좀비와 거리 비교
			FVector ZombieLocation = AIPawn->GetActorLocation();
			FVector FenceLocation = NowFence->GetActorLocation();

			float Distance = FVector::DistSquared(FenceLocation, ZombieLocation);
			
			if (Distance < MinDistance)
			{
				NearestFence=NowFence;
			}
		}
	}
	if (!NearestFence)
	{
		UE_LOG(LogTemp, Warning, TEXT("Not NearestFence"));
		return FVector::ZeroVector;
	}
	return NearestFence->GetActorLocation();
}


