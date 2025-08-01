// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/LSShopWidget.h"
#include "Components/Button.h"

void ULSShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (BuyButton)
	{
		BuyButton->OnClicked.AddDynamic(this,&ULSShopWidget::OnBuyClicked);
	}
}

void ULSShopWidget::OnBuyClicked()
{
}
