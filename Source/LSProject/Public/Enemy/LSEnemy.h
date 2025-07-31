#pragma once

#include "CoreMinimal.h"
#include "Character/LSCharacterBase.h"
#include "LSEnemy.generated.h"

UCLASS()
class LSPROJECT_API ALSEnemy : public ALSCharacterBase
{
	GENERATED_BODY()

public:
	ALSEnemy();

protected:
	void Attack();
	void Death();
	virtual void EnemyDelete();
	
	float Damage;
};
