// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/LSShopWidget.h"

#include "Character/LSPlayerCharacter.h"
#include "Components/Button.h"
#include "DataTable/LSShopItemRow.h"
#include "Widget/LSBuyButtonWidget.h"
#include "Components/TextBlock.h"
#include "Component/LSShopComp.h"
#include "Character/LSPlayerCharacter.h"

void ULSShopWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Pawn))
		{
			ShopComp = Pawn->FindComponentByClass<ULSShopComp>();
			if (!ShopComp)
				UE_LOG(LogTemp, Warning, TEXT("ShopComp not found on Pawn"));
		}
	}

	
}

void ULSShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!WeaponContainer||!AttachmentContainer||!ItemContainer)	return;
	
	const FString Context = TEXT("ULSStoreWidget::NativeConstruct");
	
	const TArray<FName> RowNames = ShopItemTable->GetRowNames();
	for (const FName& RowName : RowNames)
	{
		const FLSShopItemRow* Row = ShopItemTable->FindRow<FLSShopItemRow>(RowName, Context, true);
		if (!Row) continue;

		ULSBuyButtonWidget* Btn = CreateWidget<ULSBuyButtonWidget>(this, BuyButtonWidgetClass);
		if (!Btn) continue;

		if (Row->ShopCategory==FName(TEXT("Weapon")))
		{
			FText NText=FText::FromName(Row->Name);
			Btn->NameText->SetText(NText);

			FText PText=FText::AsNumber(Row->Price);
			Btn->PriceText->SetText(PText);
		
			WeaponContainer->AddChild(Btn);
		}
		else if (Row->ShopCategory==FName(TEXT("Attachment")))
		{
			FText NText=FText::FromName(Row->Name);
			Btn->NameText->SetText(NText);

			FText PText=FText::AsNumber(Row->Price);
			Btn->PriceText->SetText(PText);
		
			AttachmentContainer->AddChild(Btn);
		}
		else if (Row->ShopCategory==FName(TEXT("Item")))
		{
			FText NText=FText::FromName(Row->Name);
			Btn->NameText->SetText(NText);

			FText PText=FText::AsNumber(Row->Price);
			Btn->PriceText->SetText(PText);
		
			ItemContainer->AddChild(Btn);
		}
		

		Btn->OnBuyClicked.AddUniqueDynamic(this, &ULSShopWidget::HandleBuyClicked);
	}
}

void ULSShopWidget::HandleBuyClicked(const FName& Name)
{
	ShopComp->BuyItem(Name);

	//UE_LOG(LogTemp, Warning, TEXT("Shop Ui Button Clicked: %s"),*Name.ToString());
}

