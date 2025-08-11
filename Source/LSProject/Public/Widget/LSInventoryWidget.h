// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSInventoryWidget.generated.h"

class ULSInvenSlot;

/**
 * 
 */
UCLASS()
class LSPROJECT_API ULSInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Store")
	TSubclassOf<ULSInvenSlot> InvenSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPanelWidget> InvenContainer;

private:
	void LayoutInUI();
};
