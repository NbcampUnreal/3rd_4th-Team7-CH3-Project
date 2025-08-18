// Fill out your copyright notice in the Description page of Project Settings.

#include "Component//LSShopComp.h"
#include "Controller/LSPlayerController.h"
#include "Game/LSPlayerState.h"
#include "DataTable/LSShopItemRow.h"
#include "Engine/DataTable.h"
#include "Component/LSInventoryComp.h"
#include "Character/LSPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

ULSShopComp::ULSShopComp()
{
	PrimaryComponentTick.bCanEverTick = true;

	ShopItemData=CreateDefaultSubobject<UDataTable>(TEXT("ShopItemData"));

	Inventory=nullptr;
}


void ULSShopComp::BeginPlay()
{
	Super::BeginPlay();

	if (AActor* Owner=GetOwner())
	{
		if (ALSPlayerCharacter* Player=Cast<ALSPlayerCharacter>(Owner))
		{
			ULSInventoryComp* InvenComp=Player->FindComponentByClass<ULSInventoryComp>();
			if (InvenComp)
			{
				Inventory=InvenComp;
			}
				
		}
	}
}

void ULSShopComp::BuyItem(const FName& ItemName)
{
	if (!ShopItemData)	return;
	
	if (ALSPlayerController* PC=Cast<ALSPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if (ALSPlayerState* PS=PC->GetPlayerState<ALSPlayerState>())
		{
			const FString Context(TEXT("Get Item"));
			const FLSShopItemRow* Row=ShopItemData->FindRow<FLSShopItemRow>(ItemName,Context,true);
			if (Row)
			{
				int32 ItemPrice=Row->Price;

				if (PS->AddCoin(-ItemPrice))
				{
					Inventory->AddToInven(ItemName,Row->Number);
					UGameplayStatics::PlaySound2D(this, ClickSound);
				}
				else
				{
					OnShopNotEnoughMoney.Broadcast();
				}
				
			}
			
		}
	}
}
