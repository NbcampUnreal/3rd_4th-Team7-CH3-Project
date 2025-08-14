// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSInventoryWidget.generated.h"

class ULSInvenSlot;

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdateInvenUI);

UCLASS()
class LSPROJECT_API ULSInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;

public:
	FOnUpdateInvenUI OnUpdateInvenUI;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inven")
	TSubclassOf<ULSInvenSlot> InvenSlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inven")
	TSubclassOf<ULSInvenSlot> WeaponSlotClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inven")
	TSubclassOf<ULSInvenSlot> EmptySlotClass;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPanelWidget> InvenContainer;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPanelWidget> WeaponContainer;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UPanelWidget> EmptyContainer;
	
	UPROPERTY(EditDefaultsOnly, Category="Inven")
	UDataTable* InventoryTable;

	UFUNCTION()
	void UpdateSlot();
	
private:
	UPROPERTY()																	
	TObjectPtr<ULSInvenSlot> WeaponSlot;

	UPROPERTY()																	
	TObjectPtr<ULSInvenSlot> EmptySlot;

	void LayOutEmptyInven();
	void LayoutInInven();
	void LayOutInWeapon();
	
};
