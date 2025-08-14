#include "Game/LSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "System/LSDayNightController.h"
#include "Controller/LSPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EngineUtils.h"                
#include "InterchangeResult.h"
#include "Enemy/LSEnemySpawnVolume.h"
#include "Enemy/LSBoss.h"
#include "Components/BoxComponent.h"
#include "Game/LSPlayerState.h"
#include "Character/LSPlayerCharacter.h"



void ALSGameState::BeginPlay()
{
	Super::BeginPlay();

	DayNightCtrl = Cast<ALSDayNightController>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALSDayNightController::StaticClass())
	);
	TryRegisterSpawnVolumes();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle, this, &ALSGameState::UpdateHUD, 0.2f, true, 0.0f
	);

	UpdateHUD();
	
}

ALSGameState::ALSGameState()
{
	bIsCharacterOverlappedWithDoor=false;
	bIsDay=true;
	//bIShowingShopUI=false;
}

bool ALSGameState::bGetCanOpenShopUI()
{
	if (bIsCharacterOverlappedWithDoor && bIsDay)
	{
		return true;
	}
	
	return false;
}


void ALSGameState::UpdateHUD()
{
	if (!DayNightCtrl) return;
	if (HasAuthority())
	{
		const bool bIsDayNow = DayNightCtrl->IsDayPhase();
		const int32 DayNow = DayNightCtrl->GetCurrentDay();
		if (bIsDayNow != bPrevIsDay || DayNow != PrevDay)
		{
			if (!bIsDayNow)   StartNightWave(DayNow); // 밤 시작
			else              EndWave();           // 낮 시작(스폰 정지)

			bPrevIsDay = bIsDayNow;
			PrevDay    = DayNow;
		}
	}
	auto* PC = Cast<ALSPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return;

	UUserWidget* HUD = PC->GetHUDWidget();
	if (!HUD) return;
	
	if (!DayTextBlock) 
		DayTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("DayText")));
	if (!TimeTextBlock)
		TimeTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("TimeText")));
	if (!CoinTextBlock)
		CoinTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("CoinText")));
	if (!KillTextBlock)
		KillTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("KillText")));
	if (!ShopPressTextBlock)
		ShopPressTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("ShopPressText")));
	if (!DayTextBlock || !TimeTextBlock || ! ShopPressTextBlock) return;
	
	//데이 텍스트 업데이트
	const int32 Day = DayNightCtrl->GetCurrentDay();
	DayTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Day %d"), Day)));

	const int32 Secs = DayNightCtrl->GetRemainSecond();
	const int32 M = Secs / 60;
	const int32 S = Secs % 60;
	const FString Phase = DayNightCtrl->IsDayPhase() ? TEXT("DayTime") : TEXT("NightTime");
	TimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s %02d:%02d"), *Phase, M, S)));

	if (CoinTextBlock)
	{
		if (ALSPlayerState* PS = PC->GetPlayerState<ALSPlayerState>())
		{
			CoinTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d$"), PS->GetCoin())));
		}
		else
		{
			CoinTextBlock->SetText(FText::FromString(TEXT("0$")));
		}
	}
	if (KillTextBlock)
	{
		int32 Kills = 0;
		if (ALSPlayerState* PS = PC->GetPlayerState<ALSPlayerState>())
			Kills = PS->GetZombieNum();

		KillTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Kills : %d / 100"), Kills)));
	}
}
void ALSGameState::TryRegisterSpawnVolumes()
{
	if (SpawnVolumes.Num() > 0) return;

	for (TActorIterator<ALSEnemySpawnVolume> It(GetWorld()); It; ++It)
	{
		SpawnVolumes.Add(*It);
	}
}

void ALSGameState::StartNightWave(int32 Day)
{
	if (bWaveActive) return;

	bWaveActive = true;
	bBossWave   = (Day == 5);

	RemainingToSpawn = DaySpawnBudget.IsValidIndex(Day) ? DaySpawnBudget[Day] : 0;

	if (bBossWave)
	{
		TryRegisterSpawnVolumes();
		TArray<ALSEnemySpawnVolume*> Valid;
		for (auto& W : SpawnVolumes) if (W.IsValid()) Valid.Add(W.Get());

		if (Valid.Num() > 0)
		{
			ALSEnemySpawnVolume* V = Valid[FMath::RandRange(0, Valid.Num()-1)];

			const FVector Extent = V->BoxComponent->GetScaledBoxExtent();
			const FVector Center = V->BoxComponent->GetComponentLocation();
			const FVector SpawnLoc = Center + FVector(
				FMath::FRandRange(-Extent.X, Extent.X),
				FMath::FRandRange(-Extent.Y, Extent.Y),
				FMath::FRandRange(-Extent.Z, Extent.Z)
			);

			GetWorld()->SpawnActor<ALSBoss>(ALSBoss::StaticClass(), SpawnLoc, FRotator::ZeroRotator);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[Wave] No SpawnVolume found for Boss"));
		}

		// 보스 1마리로 끝
		RemainingToSpawn = 0;
		bWaveActive = false;
		return;
	}

	if (RemainingToSpawn > 0)
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimerHandle, this, &ALSGameState::SpawnTick, SpawnInterval, true);
	}
}

void ALSGameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	bWaveActive = false;
	bBossWave   = false;
	RemainingToSpawn = 0;
	DespawnRemainZombie();
}

void ALSGameState::SpawnTick()
{
	if (RemainingToSpawn <= 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	TryRegisterSpawnVolumes();
	TArray<ALSEnemySpawnVolume*> Valid;
	for (auto& W : SpawnVolumes) if (W.IsValid()) Valid.Add(W.Get());
	if (Valid.Num() == 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	ALSEnemySpawnVolume* V = Valid[FMath::RandRange(0, Valid.Num()-1)];
	const int32 CurrentDay = DayNightCtrl ? DayNightCtrl->GetCurrentDay() : PrevDay;
	V->SpawnEnemy(CurrentDay);

	--RemainingToSpawn;

	if (RemainingToSpawn <= 0)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	}
}


void ALSGameState::OnEnemyKilled()
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (ALSPlayerState* PS = Cast<ALSPlayerState>(PC->PlayerState))
		{
			PS->AddZombieKill();
			const int32 Kills = PS->GetZombieNum();
			if (Kills >= 100)
			{
				if (ALSPlayerCharacter* PlayerCharacter = Cast<ALSPlayerCharacter>(PC->GetPawn()))
				{
					const bool bAlive = (PlayerCharacter->GetCurrentHealth() > 0.f);
					if (bAlive)
					{
						if (ALSPlayerController* LSPC = Cast<ALSPlayerController>(PC))
						{
							LSPC->ShowGameClearWidget();
						}
					}
				}
			}
		}
	}
	UpdateHUD();
	
	if (bIsDay && bIsCharacterOverlappedWithDoor)
	{
		ShopPressTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ShopPressTextBlock->SetVisibility(ESlateVisibility::Hidden);	
	}
}
void ALSGameState::DespawnRemainZombie()
{
	if (!HasAuthority()) return;

	int32 Removed = 0;
	for (TActorIterator<ALSEnemy> It(GetWorld()); It; ++It)
	{
		ALSEnemy* Enemy = *It;
		if (!Enemy) continue;
		if (Enemy->IsA(ALSBoss::StaticClass()))
		{
			continue;
		}
		Enemy->GetWorldTimerManager().ClearAllTimersForObject(Enemy);

		Enemy->Destroy();
		++Removed;
	}

	AliveEnemies     = 0;
	RemainingToSpawn = 0;
}