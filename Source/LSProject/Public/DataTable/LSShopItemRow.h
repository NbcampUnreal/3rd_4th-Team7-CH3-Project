// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Engine/DataTable.h"
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
	FName Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Price;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ShopCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Number;
};
