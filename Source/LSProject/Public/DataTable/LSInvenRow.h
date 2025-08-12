// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSInvenRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FLSInvenRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Name=TEXT("None");

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon=nullptr;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Category=TEXT("None");
};
