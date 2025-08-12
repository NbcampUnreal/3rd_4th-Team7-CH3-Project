#include "Enemy/LSAttackNotify.h"

#include "Enemy/LSEnemy.h"

void ULSAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation,EventReference);
	if (AActor* Actor = MeshComp->GetOwner())
	{
		ALSEnemy* Enemy = Cast<ALSEnemy>(Actor);
		if (Enemy)
		{
			Enemy->HitAttack();
		}
	}
}
