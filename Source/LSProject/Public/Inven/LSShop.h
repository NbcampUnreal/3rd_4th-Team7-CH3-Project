
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSShop.generated.h"

UCLASS()
class LSPROJECT_API ALSShop : public AActor
{
	GENERATED_BODY()
	
public:	
	ALSShop();

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
	//class ALSInventory* Inventory;
};
