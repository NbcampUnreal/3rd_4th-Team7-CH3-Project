// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/LSGameState.h"
#include "Kismet/GameplayStatics.h"
#include "System/LSDayNightController.h"
#include "Controller/LSPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

void ALSGameState::BeginPlay()
{
	Super::BeginPlay();

	DayNightCtrl = Cast<ALSDayNightController>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALSDayNightController::StaticClass())
	);
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
	UE_LOG(LogTemp, Warning, TEXT("[HUD] DNC=%p"), DayNightCtrl);
	auto* PC = Cast<ALSPlayerController>(GetWorld()->GetFirstPlayerController());
	UE_LOG(LogTemp, Warning, TEXT("[HUD] PC=%p"), PC);
	if (!PC) return;

	UUserWidget* HUD = PC->GetHUDWidget();
	if (!HUD) return;

	//텍스트 블록 캐시
	if (!DayTextBlock) 
		DayTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("DayText")));
	if (!TimeTextBlock)
		TimeTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("TimeText")));
	if (!ShopPressTextBlock)
		ShopPressTextBlock = Cast<UTextBlock>(HUD->GetWidgetFromName(TEXT("ShopPressText")));
	if (!DayTextBlock || !TimeTextBlock || ! ShopPressTextBlock) return;
	
	//데이 텍스트 업데이트
	const int32 Day = DayNightCtrl->GetCurrentDay();
	DayTextBlock->SetText(FText::FromString(FString::Printf(TEXT("Day %d"), Day)));

	//시간텍스트 업데이트
	const int32 Secs = DayNightCtrl->GetRemainSecond();
	const int32 M = Secs / 60;
	const int32 S = Secs % 60;
	const FString Phase = DayNightCtrl->IsDayPhase() ? TEXT("낮") : TEXT("밤");
	TimeTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%s %02d:%02d"), *Phase, M, S)));
	
	if (bIsDay && bIsCharacterOverlappedWithDoor)
	{
		ShopPressTextBlock->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		ShopPressTextBlock->SetVisibility(ESlateVisibility::Hidden);	
	}
}