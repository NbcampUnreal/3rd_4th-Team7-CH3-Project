#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "LSGameState.generated.h"

class ALSDayNightController;
class UTextBlock;
class ALSEnemySpawnVolume;
class UAudioComponent;
class USoundBase;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Audio")
	UAudioComponent* BGMComp = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Audio")
	USoundBase* StartMapBGM = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Audio")
	USoundBase* DayBGM = nullptr;

	UPROPERTY(VisibleAnywhere, Category="Audio")
	USoundBase* NightBGM = nullptr;
	
	bool bGetCanOpenShopUI();

	virtual void BeginPlay() override;
    
	UFUNCTION()
	void UpdateHUD();

	UFUNCTION()
	void OnEnemyKilled();
private:
	bool bIsCharacterOverlappedWithDoor;
	bool bIsDay;

	FTimerHandle HUDUpdateTimerHandle;
	UPROPERTY() ALSDayNightController* DayNightCtrl = nullptr;

	UPROPERTY() UTextBlock* DayTextBlock = nullptr;
	UPROPERTY() UTextBlock* TimeTextBlock = nullptr;
	UPROPERTY() UTextBlock* CoinTextBlock = nullptr;
	UPROPERTY() UTextBlock* KillTextBlock = nullptr;
	UPROPERTY() UTextBlock* BulletTextBlock = nullptr;

	UPROPERTY()
	TArray<TWeakObjectPtr<ALSEnemySpawnVolume>> SpawnVolumes;
	UPROPERTY() UTextBlock* ShopPressTextBlock = nullptr;
	
	FTimerHandle SpawnTimerHandle;
	
	UPROPERTY(EditAnywhere, Category="Wave")
	float SpawnInterval = 0.1f;

	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<int32> DaySpawnBudget = {0, 10, 15, 20, 25, 30}; //총 100으로
	
	bool  bPrevIsDay = true;
	int32 PrevDay = 1;
	
	bool  bWaveActive = false;
	int32 RemainingToSpawn = 0;
	int32 AliveEnemies     = 0;
	
	void TryRegisterSpawnVolumes();
	void StartNightWave(int32 Day);
	void EndWave();
	void SpawnTick();
	void DespawnRemainZombie();

	bool bLocalPrevIsDay = true;
	int32 LocalPrevDay = 1;
	bool bLocalClearShown = false;

	void PlayBGM(USoundBase* Sound, float FadeTime = 1.0f, float Volume = 0.6f);
	void InitStartMapBGM();
	void InitMainMapBGM();

	UPROPERTY()
	USoundBase* CurrentBGMSound = nullptr;
};