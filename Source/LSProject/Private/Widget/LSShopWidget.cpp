// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/LSShopWidget.h"
#include "Components/Button.h"
#include "DataTable/LSShopItemRow.h"
#include "Widget/LSBuyButtonWidget.h"

void ULSShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void ULSShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	const FString Context = TEXT("ULSStoreWidget::NativeConstruct");
	
	const TArray<FName> RowNames = ShopItemTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FLSShopItemRow* Row = ShopItemTable->FindRow<FLSShopItemRow>(RowName, Context, true);
		if (!Row) continue;

		ULSBuyButtonWidget* Btn = CreateWidget<ULSBuyButtonWidget>(this, BuyButtonWidgetClass);
		if (!Btn) continue;
		
	}
}

void ULSShopWidget::HandleBuyClicked()
{
}

