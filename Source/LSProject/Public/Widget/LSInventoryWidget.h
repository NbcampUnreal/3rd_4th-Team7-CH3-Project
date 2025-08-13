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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inven")
	TSubclassOf<ULSInvenSlot> InvenSlotClass;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPanelWidget> InvenContainer;
	
	UPROPERTY(EditDefaultsOnly, Category="Inven")
	UDataTable* InventoryTable;
private:
	void LayoutInUI();
};
