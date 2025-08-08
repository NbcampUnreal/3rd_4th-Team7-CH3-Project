#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LSEnemy.h"
#include "LSEnemySpawnRow.generated.h"

USTRUCT(BlueprintType)
struct FLSEnemySpawnRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EnemyWave = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELSZombieType EnemyType = ELSZombieType::Normal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddHealth = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AddDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SpawnActor = nullptr;
};
