#include "AI/LSBTTask_CanGoToPlayer.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ULSBTTask_CanGoToPlayer::ULSBTTask_CanGoToPlayer()
{
	NodeName = "Is CanGoTo Player";
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] LSBTTask CanGoToPlayer is Created"));
}

EBTNodeResult::Type ULSBTTask_CanGoToPlayer::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AAIController* AIController = Comp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] AIController is NULL"));
		return EBTNodeResult::Failed;
	}
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] AIPawn is NULL"));
		return EBTNodeResult::Failed;
	}
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIPawn->GetWorld(),0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] PlayerPawn is NULL"));
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(AIPawn->GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] NavSystem is NULL"));
		return EBTNodeResult::Failed;
	}
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FNavLocation PlayerLocationNav;
	
	if (NavSystem->ProjectPointToNavigation(PlayerLocation, PlayerLocationNav, FVector(5.0f,5.0f,100.0f)))
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(PlayerLocationNav.Location);
		MoveRequest.SetAcceptanceRadius(50.0f);

		FPathFindingQuery Query;
		
		if (AIController->BuildPathfindingQuery(MoveRequest, Query))
		{
			if (NavSystem->TestPathSync(Query))
			{
				UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Find Player Location is SUCCEEDED"));
				Comp.GetBlackboardComponent()->SetValueAsVector(TEXT("MoveToLocation"), PlayerPawn->GetActorLocation());
				AIController->MoveToActor(PlayerPawn,30.f,true);
				
				return EBTNodeResult::Succeeded;
			}
			UE_LOG(LogTemp,Warning,TEXT("Player Path Is None"));
		}
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] NavSystem->Valid Query if false"));
		return EBTNodeResult::Failed;
	}
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Player Not Find"));
	return EBTNodeResult::Failed;
}
