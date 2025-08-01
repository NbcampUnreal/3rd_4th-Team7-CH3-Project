#include "Enemy/LSBTTask_CanGoToPlayer.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

ULSBTTask_CanGoToPlayer::ULSBTTask_CanGoToPlayer()
{
	NodeName = TEXT("Is CanGoTo Player");
	IsCanGoToPlayerKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(ULSBTTask_CanGoToPlayer,IsCanGoToPlayerKey));
}

EBTNodeResult::Type ULSBTTask_CanGoToPlayer::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AAIController* AIController = Comp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn) return EBTNodeResult::Failed;
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(AIPawn->GetWorld(),0);
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is NULL"));
		Comp.GetBlackboardComponent()->SetValueAsBool(IsCanGoToPlayerKey.SelectedKeyName, false);
		return EBTNodeResult::Succeeded;
	}
	
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(AIPawn->GetWorld());
	if (!NavSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("NavSystem is NULL"));
		Comp.GetBlackboardComponent()->SetValueAsBool(IsCanGoToPlayerKey.SelectedKeyName, false);
		return EBTNodeResult::Failed;
	}
	
	//EnemyTodo : Player에게 가는 길이 있는지 확인
	bool bCanGoPlayer = false;
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FNavLocation PlayerLocationNav;
	
	if (NavSystem->ProjectPointToNavigation(PlayerLocation, PlayerLocationNav, FVector(5.0f,5.0f,100.0f)))
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalLocation(PlayerLocationNav.Location);
		MoveRequest.SetAcceptanceRadius(50.0f);

		FPathFindingQuery Query;
		bool bValidQuery = AIController->BuildPathfindingQuery(MoveRequest, Query);
		if (bValidQuery)
		{
			bCanGoPlayer = NavSystem->TestPathSync(Query);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[LSEnemy]NavSystem->Valid Query if false"));
			bCanGoPlayer=false;
			Comp.GetBlackboardComponent()->SetValueAsBool(IsCanGoToPlayerKey.SelectedKeyName, bCanGoPlayer);
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		Comp.GetBlackboardComponent()->SetValueAsBool(IsCanGoToPlayerKey.SelectedKeyName, bCanGoPlayer);
		return EBTNodeResult::Failed;
	}
	
	Comp.GetBlackboardComponent()->SetValueAsBool(IsCanGoToPlayerKey.SelectedKeyName, bCanGoPlayer);
	return EBTNodeResult::Succeeded;
}
