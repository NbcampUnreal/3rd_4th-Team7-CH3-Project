// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LSInvenSlotInfo.generated.h"

/**
 * 
 */

class UTextBlock;
class UImage;

UCLASS()
class LSPROJECT_API ULSInvenSlotInfo : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	UTextBlock* NameText;

	UPROPERTY()
	UTextBlock* CountText;

	UPROPERTY()
	UImage* IconImage;

	UPROPERTY()
	FName Type;
	
};
