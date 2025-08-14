#include "Controller/LSZombieAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"

ALSZombieAIController::ALSZombieAIController()
{
	AIPerception=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	
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
	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ALSZombieAIController::OnPerceptionUpdated);
	}
}

//EnemyTodo : 일단 무시
void ALSZombieAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (Actor != PlayerPawn || !BlackboardComp)
	{
		return;
	}
	if (Stimulus.WasSuccessfullySensed())
	{
		// Blackboard에 정보 저장
		BlackboardComp->SetValueAsObject(TEXT("TargetActor"), Actor);
	}
}
