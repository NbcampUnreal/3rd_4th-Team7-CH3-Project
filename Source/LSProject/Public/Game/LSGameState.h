#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LSGameState.generated.h"

class ALSDayNightController;
class UTextBlock;
class ALSEnemySpawnVolume;

UCLASS()
class LSPROJECT_API ALSGameState : public AGameState
{
	GENERATED_BODY()

public:
	ALSGameState();
	FORCEINLINE void SetDoorOverlapped(bool bIsOverlapped){bIsCharacterOverlappedWithDoor=bIsOverlapped;}
	FORCEINLINE void SetDay(bool bDay){bIsDay=bDay;}

	FORCEINLINE bool GetDoorOverlapped() const {return bIsCharacterOverlappedWithDoor;}
	FORCEINLINE bool GetDay() const {return bIsDay;}

	bool bGetCanOpenShopUI();

	virtual void BeginPlay() override;
    
	UFUNCTION()
	void UpdateHUD();

	UFUNCTION()
	void OnEnemyKilled();
private:
	bool bIsCharacterOverlappedWithDoor;
	bool bIsDay;
	//bool bIShowingShopUI;
	bool bCanUseShop;

	FTimerHandle HUDUpdateTimerHandle;
	UPROPERTY() ALSDayNightController* DayNightCtrl = nullptr;

	UPROPERTY() UTextBlock* DayTextBlock = nullptr;
	UPROPERTY() UTextBlock* TimeTextBlock = nullptr;
	UPROPERTY() UTextBlock* CoinTextBlock = nullptr;
	UPROPERTY() UTextBlock* KillTextBlock = nullptr;

	UPROPERTY()
	TArray<TWeakObjectPtr<ALSEnemySpawnVolume>> SpawnVolumes;
	UPROPERTY() UTextBlock* ShopPressTextBlock = nullptr;
	
	FTimerHandle SpawnTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Wave")
	float SpawnInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<int32> DaySpawnBudget = {0, 14, 25, 35, 45, 1}; //총 120으로
	
	bool  bPrevIsDay = true;
	int32 PrevDay = 1;
	
	bool  bWaveActive = false;
	bool  bBossWave   = false;
	int32 RemainingToSpawn = 0;
	int32 AliveEnemies     = 0;
	
	void TryRegisterSpawnVolumes();
	void StartNightWave(int32 Day);
	void EndWave();
	void SpawnTick();
	
};