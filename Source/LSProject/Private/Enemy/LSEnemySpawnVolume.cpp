#include "Enemy/LSEnemySpawnVolume.h"

#include "Enemy/LSEnemySpawnRow.h"
#include "Components/BoxComponent.h"

ALSEnemySpawnVolume::ALSEnemySpawnVolume()
{
	SceneComponent=CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent=SceneComponent;
	BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);
	TempWave = 1;
}

void ALSEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
}

void ALSEnemySpawnVolume::SpawnEnemy(int32 NowWave) const
{
	if (FLSEnemySpawnRow* SpawnRow = GetRandomEnemy(NowWave))
	{
		if (TSubclassOf<AActor> SpawnActorClass = SpawnRow->SpawnActor)
		{
			ALSEnemy* Enemy = Cast<ALSEnemy>(GetWorld()->SpawnActor<AActor>(
				SpawnActorClass,
				GetRandomVector(),
				FRotator::ZeroRotator
			));
			
			if (Enemy && SpawnRow)
			{
				float AddHealth = SpawnRow->AddHealth;
				float AddDamage = SpawnRow->AddDamage;
				Enemy->AddAbility(AddHealth, AddDamage);
			}
		}
	}
}

FLSEnemySpawnRow* ALSEnemySpawnVolume::GetRandomEnemy(int32 NowWave) const
{
	if (!EnemyDataTable) return nullptr;
	TArray<FLSEnemySpawnRow*> AllRows;
	FString ContextString = TEXT("EnemySpawnContext");

	EnemyDataTable->GetAllRows(ContextString, AllRows);

	if (!AllRows.IsEmpty())
	{
		float CompareRate = 0.0f;
		for (FLSEnemySpawnRow *Row : AllRows)
		{
			if (Row->EnemyWave!=NowWave) continue;
			if(Row->SpawnRate)
			{
				float SpawnRandomRate = FMath::RandRange(0.0f,100.0f);
				CompareRate += Row->SpawnRate;
				if (SpawnRandomRate<=CompareRate)
				{
					return Row;
				}
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("[LSEnemyLog] Return Random Enemy is FAILED"))
	return nullptr;
}

FVector ALSEnemySpawnVolume::GetRandomVector() const
{
	FVector Extent = BoxComponent->GetScaledBoxExtent();
	FVector CenterLocation = BoxComponent->GetComponentLocation();
	
	return CenterLocation + FVector(
		FMath::RandRange(-Extent.X, Extent.X),
		FMath::RandRange(-Extent.Y, Extent.Y),
		FMath::RandRange(-Extent.Z, Extent.Z)
	);
}
