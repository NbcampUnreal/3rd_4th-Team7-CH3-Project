#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSEnemySpawnRow.h"
#include "LSEnemySpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class LSPROJECT_API ALSEnemySpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSEnemySpawnVolume();
	void SpawnEnemy(int32 NowWave) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	TObjectPtr<UDataTable> EnemyDataTable;
	
protected:
	virtual void BeginPlay() override;

	FLSEnemySpawnRow* GetRandomEnemy(int32 WaveNum) const;
	
	FVector GetRandomVector() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Wave")
	int32 TempWave;
};
