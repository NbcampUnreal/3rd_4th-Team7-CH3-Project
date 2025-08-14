// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LSInvenSlot.h"

#include "DataTable/LSDragDropOperation.h"
#include "DataTable/LSInvenRowObject.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Controller/LSPlayerController.h"
#include "Widget/LSInventoryWidget.h"
#include "Character/LSPlayerCharacter.h"
#include "Component/LSInventoryComp.h"
#include "Components/TextBlock.h"

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
	RowObject->Type=Type;

	UDragDropOperation* Op=NewObject<UDragDropOperation>();
	Op->Payload=RowObject;
	Op->DefaultDragVisual=this;
	Op->Pivot=EDragPivot::MouseDown;

	OutOperation=Op;
	
}


bool ULSInvenSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                                UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	if (!InOperation)	return false;

	ULSInvenRowObject* RowObj = Cast<ULSInvenRowObject>(InOperation->Payload);
	if (!RowObj)	return false;

	ALSPlayerController* PC=GetOwningPlayer<ALSPlayerController>();
	if (!PC)	return false;

	APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn)	return false;

	ALSPlayerCharacter* Player = Cast<ALSPlayerCharacter>(Pawn);
	if (!Player)	return false;

	ULSInventoryComp* InvenComp=Player->FindComponentByClass<ULSInventoryComp>();
	if (!InvenComp)	return false;

	UE_LOG(LogTemp, Warning, TEXT("Slot name: %s"), *SlotName.ToString());
	
	if (SlotName=="Weapon" && RowObj->Type=="Weapon")
	{
		NameText=RowObj->NameText;
		CountText=RowObj->CountText;
		IconImage=RowObj->IconImage;
		Type=RowObj->Type;

		UE_LOG(LogTemp, Warning, TEXT("My Weapon Type: %s"), *Type.ToString());
		
		FText TextName=NameText->GetText();
		FString TextAsString=TextName.ToString();
		FName TextAsName(*TextAsString);
		
		//같은 무기일 경우 예외처리
		if (InvenComp->GetMyWeapon()==TextAsName)	return false;

		//인벤토리로 복구
		if (Type=="Weapon")
		{
			InvenComp->Unequip();
		}
		
		//원래 있던 슬롯에서 -1
		InvenComp->equip(TextAsName);
		
		//Update Layout
		if (!PC->GetInvenWidget())	return false;
		PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
		return true;
	}
	
	if (SlotName=="Inventory")
	{
		UE_LOG(LogTemp, Warning, TEXT("To Type: %s"), *Type.ToString());
		UE_LOG(LogTemp, Warning, TEXT("from Type: %s"), *RowObj->Type.ToString());
		
		if (Type=="Weapon" && RowObj->Type=="Weapon")
		{
			NameText=RowObj->NameText;
			CountText=RowObj->CountText;
			IconImage=RowObj->IconImage;
			Type=RowObj->Type;

			FText TextName=NameText->GetText();
			FString TextAsString=TextName.ToString();
			FName TextAsName(*TextAsString);
			InvenComp->ChangeWeaponSlot(TextAsName);
			
			if (!PC->GetInvenWidget())	return false;
			PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
			return true;
		}
	}

	if (SlotName==FName(TEXT("Empty")))
	{
		NameText=RowObj->NameText;
		CountText=RowObj->CountText;
		IconImage=RowObj->IconImage;
		Type=RowObj->Type;
		
		InvenComp->Unequip();
		
		//Update Layout
		if (!PC->GetInvenWidget())	return false;
		PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
		return true;
	}
	
	return false;
}

