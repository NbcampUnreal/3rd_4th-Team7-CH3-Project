// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSShopWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class LSPROJECT_API ULSShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

public:
	UPROPERTY(meta=(BindWidget))
	UButton* BuyButton;

	UFUNCTION()
	void OnBuyClicked();
};
