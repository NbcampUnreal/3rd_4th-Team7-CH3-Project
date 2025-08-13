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

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
public:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta=(BindWidget))
	UImage* IconImage;

	FORCEINLINE void SetType(FName Category){Type=Category;}
	
private:
	FName Type;
};
