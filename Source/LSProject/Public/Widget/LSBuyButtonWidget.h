// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Component/LSShopComp.h"
#include "DataTable/LSShopItemRow.h"
#include "LSBuyButtonWidget.generated.h"

class UButton;
class UTextBlock;
class UImage;

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuyClicked,const FName&,Row);

UCLASS()
class LSPROJECT_API ULSBuyButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	//UPROPERTY(BlueprintAssignable)
	FOnBuyClicked OnBuyClicked;

protected:
	virtual void NativeConstruct() override;
	
public:
	FLSShopItemRow RowData;

	UPROPERTY(meta=(BindWidget))
	UButton* BuyButton;
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* PriceText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* NumText;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* IconImage;

	UFUNCTION()
	void ButtonClicked();
};
