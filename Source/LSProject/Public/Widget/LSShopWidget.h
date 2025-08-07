// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EntitySystem/MovieSceneComponentPtr.h"
#include "LSShopWidget.generated.h"

class UButton;
class UTextBlock;
class UDataTable;
class ULSBuyButtonWidget;
class ULSShopComp;
/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class LSPROJECT_API ULSShopWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, Category="Store")
	UDataTable* ShopItemTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Store")
	TSubclassOf<ULSBuyButtonWidget> BuyButtonWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UPanelWidget* WeaponContainer;

	UPROPERTY(meta=(BindWidget))
	UPanelWidget* AttachmentContainer;

	UPROPERTY(meta=(BindWidget))
	UPanelWidget* ItemContainer;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> CoinText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> NotEnoughMoneyText;
	
	UFUNCTION()
	void HandleBuyClicked(const FName& Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings")
	float DurationDisappear;
private:
	UFUNCTION()
	void ShowInsufficientMoneyText();
	void HideInsufficientMoneyText();
	FTimerHandle HideNotEnoughMoneyTextTimer;
};
