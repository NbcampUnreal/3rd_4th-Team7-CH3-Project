#include "Enemy/LSBTTask_Attack.h"

#include "AIController.h"
#include "Enemy/LSEnemy.h"


ULSBTTask_Attack::ULSBTTask_Attack()
{
	NodeName ="ZombieAttack";
}

EBTNodeResult::Type ULSBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& Comp, uint8* NodeMemory)
{
	AAIController* AIController = Comp.GetAIOwner();
	if (!AIController)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BTTask_Attack AAIController Is Failed"))
		return EBTNodeResult::Failed;
	}
	APawn* AIPawn = AIController->GetPawn();
	if (!AIPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BTTask_Attack AIPawn Is Failed"))
		return EBTNodeResult::Failed;
	}
	if(ALSEnemy* Enemy = Cast<ALSEnemy>(AIPawn))
	{
		UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] BTTask_Attack : Enemy Attack() Is Called"))
		Enemy->Attack();
	}
	
	return EBTNodeResult::Succeeded;
}