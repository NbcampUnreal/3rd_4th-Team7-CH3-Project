#include "Enemy/LSEnemySpawnVolume.h"

#include "Enemy/LSEnemySpawnRow.h"
#include "Components/BoxComponent.h"

ALSEnemySpawnVolume::ALSEnemySpawnVolume()
{
	SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent=SceneComponent;
	BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	EnemyDataTable=nullptr;
}

void ALSEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALSEnemySpawnVolume::SpawnEnemy()
{
	
}

FLSEnemySpawnRow* ALSEnemySpawnVolume::GetRandomEnemy() const
{
	if (!EnemyDataTable) return nullptr;
	
	return nullptr; 
}

FVector ALSEnemySpawnVolume::GetRandomVector() const
{
	FVector Extent = BoxComponent->GetScaledBoxExtent();
	FVector CenterLocation = BoxComponent->GetComponentLocation();
	
	return CenterLocation + FVector(
		FMath::RandRange(-Extent.X, Extent.X),
		FMath::RandRange(-Extent.X, Extent.X),
		FMath::RandRange(-Extent.X, Extent.X)
	);
}
