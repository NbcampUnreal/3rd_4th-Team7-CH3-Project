// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSShopComp.generated.h"


UCLASS(ClassGroup=(Custom),Blueprintable ,meta=(BlueprintSpawnableComponent))
class LSPROJECT_API ULSShopComp : public UActorComponent
{
	GENERATED_BODY()

public:
	ULSShopComp();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void BuyItem(const FName& ItemName);
	void SellItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
	UDataTable* ShopItemData;
	
private:
	//추후 플레이어 클래스로 이동
	class ULSInventoryComp* Inventory;
};
