// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTable/LSInvenRow.h"
#include "LSInvenRowObject.generated.h"

/**
 * 
 */

class UTextBlock;
class UImage;

UCLASS()
class LSPROJECT_API ULSInvenRowObject : public UObject
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
