// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LSInvenSlot.h"

#include "DataTable/LSDragDropOperation.h"
#include "DataTable/LSInvenRowObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

FReply ULSInvenSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent, this, EKeys::LeftMouseButton);
	
	return Reply.NativeReply;
}

void ULSInvenSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	ULSInvenRowObject* RowObject=NewObject<ULSInvenRowObject>();
	RowObject->NameText=NameText;
	RowObject->CountText=CountText;
	RowObject->IconImage=IconImage;

	UDragDropOperation* Op=NewObject<UDragDropOperation>();
	Op->Payload=RowObject;
	Op->DefaultDragVisual=this;
	Op->Pivot=EDragPivot::MouseDown;

	OutOperation=Op;
	
}

bool ULSInvenSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	if (ULSInvenRowObject* RowObj = Cast<ULSInvenRowObject>(InOperation->Payload))
	{
		NameText=RowObj->NameText;
		CountText=RowObj->CountText;
		IconImage=RowObj->IconImage;
		// 슬롯 정보 갱신: RowData를 기반으로 장착/교체 처리
		
		return true;
	}
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
