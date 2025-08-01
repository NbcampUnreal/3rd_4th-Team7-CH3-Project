// Fill out your copyright notice in the Description page of Project Settings.

#include "Component//LSShopComp.h"
#include "Controller/LSPlayerController.h"
#include "Game/LSPlayerState.h"
#include "DataTable/LSShopItemRow.h"
#include "Engine/DataTable.h"

//추후 삭제
#include "Component/LSInventoryComp.h"

ULSShopComp::ULSShopComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	ShopItemData=CreateDefaultSubobject<UDataTable>(TEXT("ShopItemData"));

	//추후 이동
	Inventory=CreateDefaultSubobject<ULSInventoryComp>(TEXT("Inventory"));
}


void ULSShopComp::BeginPlay()
{
	Super::BeginPlay();

}

void ULSShopComp::BuyItem(const FName& ItemName)
{
	if (!ShopItemData)	return;
	
	if (ALSPlayerController* PC=Cast<ALSPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ALSPlayerState* PlayerState=PC->GetPlayerState<ALSPlayerState>())
		{
			int CurCoin=PlayerState->GetCoin();
			//위젯 버튼->아이템 선택

			const FString Context(TEXT("Get Item"));
			const FLSShopItemRow* Row=ShopItemData->FindRow<FLSShopItemRow>(ItemName,Context,true);
			if (Row)
			{
				int32 ItemPrice=Row->Price;

				if(CurCoin>=ItemPrice)
				{
					CurCoin-=ItemPrice;
					PlayerState->SetCoin(CurCoin);
					//PC->Player->Inventory->AddtoInven(ItemName)
					Inventory->AddToInven(ItemName);

					UE_LOG(LogTemp,Warning,TEXT("CurCoin: %d"),CurCoin);
				}
				else
				{
					//UI->돈이 부족
				}
			}
			
		}
	}
}

void ULSShopComp::SellItem()
{
}
