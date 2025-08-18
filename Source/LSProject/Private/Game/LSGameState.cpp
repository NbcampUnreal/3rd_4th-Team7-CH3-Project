#include "Game/LSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "System/LSDayNightController.h"
#include "Controller/LSPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "EngineUtils.h"                
#include "Enemy/LSEnemySpawnVolume.h"
#include "Enemy/LSBoss.h"
#include "Game/LSPlayerState.h"
#include "Character/LSPlayerCharacter.h"
#include "Weapon/LSPlayerWeaponSystemComp.h"
#include "Weapon/LSWeaponBase.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Game/LSGameMode.h"

void ALSGameState::BeginPlay()
{
	Super::BeginPlay();

	DayNightCtrl = Cast<ALSDayNightController>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALSDayNightController::StaticClass())
	);
	TryRegisterSpawnVolumes();
	if (ALSGameMode* GM = GetWorld()->GetAuthGameMode<ALSGameMode>())
	{
		if (GM->StartMapBGM) StartMapBGM = GM->StartMapBGM;
		if (GM->DayBGM)      DayBGM      = GM->DayBGM;
		if (GM->NightBGM)    NightBGM    = GM->NightBGM;
	}
	const FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	if (LevelName.Equals(TEXT("LSStartMap"), ESearchCase::IgnoreCase))
	{
		InitStartMapBGM();
	}
	else if (LevelName.Equals(TEXT("LSMainMap"), ESearchCase::IgnoreCase))
	{
		InitMainMapBGM();
	}
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

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BGMComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComp"));
	BGMComp->SetupAttachment(RootComponent);
	BGMComp->bAutoActivate = false;
	BGMComp->bAllowSpatialization = false;
	BGMComp->bIsUISound = false;
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
			PrevDay = DayNow;
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
	if (!BulletTextBlock)
		BulletTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("BulletTextBlock")));
	if (!DayTextBlock || !TimeTextBlock || !ShopPressTextBlock || !BulletTextBlock) return;

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
	int32 Kills = 0;
	if (KillTextBlock)
	{
		if (ALSPlayerState* PS = PC->GetPlayerState<ALSPlayerState>())
			Kills = PS->GetZombieNum();

		KillTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Kills : %d / 100"), Kills)));
	}

	if (bGetCanOpenShopUI())
	{
		ShopPressTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ShopPressTextBlock->SetVisibility((ESlateVisibility::Hidden));
	}

	const bool  bIsDayNow = DayNightCtrl->IsDayPhase();
	const int32 DayNow = DayNightCtrl->GetCurrentDay();
	if (!bLocalClearShown && Kills >= 100)
	{
		if (PC->IsLocalPlayerController())
		{
			PC->ShowGameClearWidget();
			bLocalClearShown = true;
		}
	}
	if ((bIsDayNow != bLocalPrevIsDay) || (DayNow != LocalPrevDay))
	{
		if (!bLocalClearShown && bIsDayNow && (LocalPrevDay == 5))
		{
			if (PC->IsLocalPlayerController())
			{
				PC->ShowGameClearWidget();
				bLocalClearShown = true;
			}
		}
		PlayBGM(bIsDayNow ? DayBGM : NightBGM, 1.2f, 0.6f);
		bLocalPrevIsDay = bIsDayNow;
		LocalPrevDay = DayNow;
	}

	
	if (BulletTextBlock)
	{
		if (ALSPlayerCharacter* Character = Cast<ALSPlayerCharacter>(PC->GetPawn()))
		{
			if (ULSPlayerWeaponSystemComp* WeaponComp = Character->FindComponentByClass<ULSPlayerWeaponSystemComp>())
			{
				int32 CurrentAmmo = 0;
				int32 MaxAmmo = 0;
				if (WeaponComp->CurrentWeapon)
				{
					CurrentAmmo = WeaponComp->CurrentWeapon->GetCurrentAmmo();
					MaxAmmo = WeaponComp->CurrentWeapon->GetMaxAmmo();
				}

				BulletTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Bullet : %d / %d"), CurrentAmmo, MaxAmmo)));
			}
		}
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
	RemainingToSpawn = DaySpawnBudget.IsValidIndex(Day) ? DaySpawnBudget[Day] : 0;
	
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
			PS->AddCoin(50);
		}
	}
	UpdateHUD();
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
void ALSGameState::PlayBGM(USoundBase* Sound, float FadeTime, float Volume)
{
	if (!BGMComp) return;

	if (Sound && Sound == CurrentBGMSound && BGMComp->IsPlaying())
		return;

	if (BGMComp->IsPlaying())
		BGMComp->FadeOut(FadeTime, 0.f);

	CurrentBGMSound = Sound;
	BGMComp->SetSound(Sound);
	if (Sound)
	{
		BGMComp->FadeIn(FadeTime, Volume);
		BGMComp->Play();
	}
}

void ALSGameState::InitStartMapBGM()
{
	PlayBGM(StartMapBGM, 3.0f, 0.5f); 
}

void ALSGameState::InitMainMapBGM()
{
	const bool bIsDayNow = DayNightCtrl ? DayNightCtrl->IsDayPhase() : true;
	PlayBGM(bIsDayNow ? DayBGM : NightBGM, 3.0f, 0.1f);
	bLocalPrevIsDay = bIsDayNow;
	LocalPrevDay = DayNightCtrl ? DayNightCtrl->GetCurrentDay() : LocalPrevDay;
}