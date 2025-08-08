#include "Controller/LSZombieAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
//#include "BehaviorTree/BehaviorTreeComponent.h"

ALSZombieAIController::ALSZombieAIController()
{
	AIPerception=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);
	
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp"));
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] AIController is born"));

}

void ALSZombieAIController::BeginPlay()
{
	Super::BeginPlay();
}

//EnemyTodo : ForceInline
UBlackboardComponent* ALSZombieAIController::GetBlackBoardComp() const
{
	return BlackboardComp;
}

void ALSZombieAIController::StartBehaviorTree()
{
	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BehaviorTreeAsset is start"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BehaviorTreeAsset NOT START"));
	}
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
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Blackboard initialized successfully"));
		StartBehaviorTree();
	}
	if (AIPerception)
	{
		AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ALSZombieAIController::OnPerceptionUpdated);
	}
}

//EnemyTodo :
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
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemy] Blackboard PerceptionUpdated successfully"));
	}	
}
