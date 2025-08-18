#include "Enemy/LSEnemyAnimNotify.h"

#include "Enemy/LSEnemy.h"

void ULSEnemyAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);
	if (AActor* Actor = MeshComp->GetOwner())
	{
		if (ALSEnemy* Enemy = Cast<ALSEnemy>(Actor))
		{
			Enemy->Attack();
		}
	}
}