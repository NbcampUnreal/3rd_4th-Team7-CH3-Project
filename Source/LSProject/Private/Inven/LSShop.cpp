
#include "Inven/LSShop.h"
#include "Controller/LSPlayerController.h"
#include "Game/LSPlayerState.h"
#include "DataTable/LSShopItemRow.h"
#include "Engine/DataTable.h"

//추후삭제
#include "Inven/LSInventory.h"

ALSShop::ALSShop()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

void ALSShop::BeginPlay()
{
	Super::BeginPlay();
}

void ALSShop::BuyItem(const FName&  ItemName)
{
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
				}
				else
				{
					//UI->돈이 부족
				}
			}
			
		}
	}
}

void ALSShop::SellItem()
{
}




