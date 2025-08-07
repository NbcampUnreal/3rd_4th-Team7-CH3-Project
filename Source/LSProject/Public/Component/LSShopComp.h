// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSShopComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnShopNotEnoughMoney);

UCLASS(ClassGroup=(Custom) ,meta=(BlueprintSpawnableComponent))
class LSPROJECT_API ULSShopComp : public UActorComponent
{
	GENERATED_BODY()

public:
	ULSShopComp();

	void BuyItem(const FName& ItemName);
	void SellItem();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	UDataTable* ShopItemData;

	FOnShopNotEnoughMoney OnShopNotEnoughMoney;
protected:
	virtual void BeginPlay() override;

private:
	//추후 플레이어 클래스로 이동
	class ULSInventoryComp* Inventory;
};
