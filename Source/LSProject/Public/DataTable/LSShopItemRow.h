// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LSShopItemRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLSShopItemRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Name=TEXT("None");

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Price=0;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ShopCategory=("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon=nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Number=0;
};
