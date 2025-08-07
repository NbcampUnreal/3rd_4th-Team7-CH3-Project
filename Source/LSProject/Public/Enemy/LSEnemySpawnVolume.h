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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	TObjectPtr<USceneComponent> SceneComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	TObjectPtr<UBoxComponent> BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component")
	TObjectPtr<UDataTable> EnemyDataTable;
	
protected:
	virtual void BeginPlay() override;

	void SpawnEnemy();
	FLSEnemySpawnRow* GetRandomEnemy() const;
	FVector GetRandomVector() const;

};
