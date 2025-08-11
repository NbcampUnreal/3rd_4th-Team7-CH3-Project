// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LSInvenSlot.generated.h"

class FTextBlock;
class UImage;

/**
 * 
 */

class UTextBlock;
class UTexture2D;
class UImage;

UCLASS()
class LSPROJECT_API ULSInvenSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Name;

	UPROPERTY(meta=(BindWidget))
	int32 Count;

	UPROPERTY(meta=(BindWidget))
	UImage* Image;
};
