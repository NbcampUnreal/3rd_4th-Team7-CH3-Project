#include "Controller/LSZombieAIController.h"

#include "BehaviorTree/BlackboardComponent.h"

ALSZombieAIController::ALSZombieAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
}

void ALSZombieAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ALSZombieAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(TEXT("IsCanGoToPlayer"), false);
		BlackboardComp->SetValueAsVector(TEXT("ClosestFenceLocation"), FVector(0.0f, 0.0f, 0.0f));
	}
}
