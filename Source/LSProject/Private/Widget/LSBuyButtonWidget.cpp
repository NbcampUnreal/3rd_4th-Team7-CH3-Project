// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LSBuyButtonWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void ULSBuyButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (BuyButton)
	{
		BuyButton->OnClicked.AddUniqueDynamic(this, &ULSBuyButtonWidget::ButtonClicked);
	}
}

void ULSBuyButtonWidget::ButtonClicked()
{
	FString Label = TEXT("");
	if (NameText)
	{
		
		Label = NameText->GetText().ToString();
	}
	OnBuyClicked.Broadcast(FName(*Label));
}
