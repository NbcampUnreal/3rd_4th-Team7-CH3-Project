// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/LSShopWidget.h"

#include "Character/LSPlayerCharacter.h"
#include "Components/Button.h"
#include "DataTable/LSShopItemRow.h"
#include "Widget/LSBuyButtonWidget.h"
#include "Components/TextBlock.h"
#include "Component/LSShopComp.h"
#include "Components/Image.h"
#include "Controller/LSPlayerController.h"
#include "Game/LSPlayerState.h"

void ULSShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn))
		{
			ULSShopComp* ShopComp = Player->FindComponentByClass<ULSShopComp>();
			if (ShopComp)
			{
				ShopComp->OnShopNotEnoughMoney.AddDynamic(this,&ULSShopWidget::ShowInsufficientMoneyText);
			}
		}

		NotEnoughMoneyText->SetVisibility(ESlateVisibility::Hidden);
	}

	SetIsFocusable(true);
}

void ULSShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetKeyboardFocus();
	
	if (!WeaponContainer||!AttachmentContainer||!ItemContainer)	return;
	
	const FString Context = TEXT("ULSStoreWidget::NativeConstruct");
	
	const TArray<FName> RowNames = ShopItemTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FLSShopItemRow* Row = ShopItemTable->FindRow<FLSShopItemRow>(RowName, Context, true);
		if (!Row) continue;

		ULSBuyButtonWidget* Btn = CreateWidget<ULSBuyButtonWidget>(this, BuyButtonWidgetClass);
		if (!Btn) continue;

		FText NText=FText::FromName(Row->Name);
		Btn->NameText->SetText(NText);

		FString PString=FString::Printf(TEXT("%d$"),Row->Price);
		Btn->PriceText->SetText(FText::FromString(PString));
		
		FString NString=FString::Printf(TEXT("X %d"),Row->Number);
		Btn->NumText->SetText(FText::FromString(NString));

		Btn->IconImage->SetBrushFromTexture(Row->Icon);
		
		if (Row->ShopCategory==FName(TEXT("Weapon")))
		{
			WeaponContainer->AddChild(Btn);
		}
		else if (Row->ShopCategory==FName(TEXT("Attachment")))
		{
			AttachmentContainer->AddChild(Btn);
		}
		else if (Row->ShopCategory==FName(TEXT("Item")))
		{
			ItemContainer->AddChild(Btn);
		}
		

		Btn->OnBuyClicked.AddUniqueDynamic(this, &ULSShopWidget::HandleBuyClicked);
	}
}

void ULSShopWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	ALSPlayerState* PS = GetOwningPlayerState<ALSPlayerState>(false);
	if (PS)
	{
		CoinText->SetText(FText::AsNumber(PS->GetCoin()));
	}
}

FReply ULSShopWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey() == EKeys::F)
	{
		ALSPlayerController* PC =Cast<ALSPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->HideShopWidget();
		}

		UE_LOG(LogTemp,Warning,TEXT("ULSShopWidget::NativeOnKeyDown"));
		
		return FReply::Handled();
	}

	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

ULSShopWidget::ULSShopWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShopItemTable=nullptr;
	DurationDisappear=1.0f;
	SetIsFocusable(true);
}

void ULSShopWidget::HandleBuyClicked(const FName& Name)
{
	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn))
		{
			ULSShopComp* ShopComp = Player->FindComponentByClass<ULSShopComp>();
			if (ShopComp)
			{
				ShopComp->BuyItem(Name);
			}
		}
	}
}

void ULSShopWidget::ShowInsufficientMoneyText()
{
	NotEnoughMoneyText->SetVisibility(ESlateVisibility::Visible);

	GetWorld()->GetTimerManager().SetTimer(
		HideNotEnoughMoneyTextTimer,
		this,
		&ULSShopWidget::HideInsufficientMoneyText,
		DurationDisappear,
		false
		);
}

void ULSShopWidget::HideInsufficientMoneyText()
{
	NotEnoughMoneyText->SetVisibility(ESlateVisibility::Hidden);
}

