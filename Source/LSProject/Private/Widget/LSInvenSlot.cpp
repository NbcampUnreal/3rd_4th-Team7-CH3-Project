// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LSInvenSlot.h"
#include "DataTable/LSInvenSlotInfo.h"
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
	
	ULSInvenSlotInfo* RowObject=NewObject<ULSInvenSlotInfo>();
	RowObject->NameText=NameText;
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

	ULSInvenSlotInfo* RowObj = Cast<ULSInvenSlotInfo>(InOperation->Payload);
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
	UE_LOG(LogTemp, Warning, TEXT("To Type: %s"), *Type.ToString());
	UE_LOG(LogTemp, Warning, TEXT("from Type: %s"), *RowObj->Type.ToString());
	
	//인벤슬롯->무기슬롯
	if (SlotName=="Weapon" && RowObj->Type=="Weapon")
	{
		UE_LOG(LogTemp, Warning, TEXT("My Weapon Type: %s"), *Type.ToString());
		
		FText TextName=RowObj->NameText->GetText();
		FString TextAsString=TextName.ToString();
		FName TextAsName(*TextAsString);
		
		//예외:같은 무기일 경우
		if (InvenComp->GetMyWeapon()==TextAsName)	return false;

		//무기->무기
		if (Type=="Weapon")
		{
			InvenComp->ChangeWeaponSlot(TextAsName);
		}
		//무기->빈
		else
		{
			InvenComp->Equip(TextAsName);
		}

		//정보 바뀜
		NameText=RowObj->NameText;
		Type=RowObj->Type;
		
		//Update Layout
		if (!PC->GetInvenWidget())	return false;
		PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
		return true;
	}

	//->인벤슬롯
	if (SlotName=="Inventory")
	{
		//무기->무기
		if (Type=="Weapon" && RowObj->Type=="Weapon")
		{
			FText TextName=NameText->GetText();
			FString TextAsString=TextName.ToString();
			FName TextAsName(*TextAsString);
			InvenComp->ChangeWeaponSlot(TextAsName);

			NameText=RowObj->NameText;
			Type=RowObj->Type;
			
			if (!PC->GetInvenWidget())	return false;
			PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
			return true;
		}
	}

	//->빈 슬롯
	if (SlotName==FName(TEXT("Empty")))
	{
		NameText=RowObj->NameText;
		Type=RowObj->Type;
		
		InvenComp->Unequip();
		
		//Update Layout
		if (!PC->GetInvenWidget())	return false;
		PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
		return true;
	}

	//->무기 사용
	if (SlotName==FName(TEXT("UseItem")) && RowObj->Type=="Item")
	{
		FText TextName=RowObj->NameText->GetText();
		FString TextAsString=TextName.ToString();
		FName TextAsName(*TextAsString);
		InvenComp->UseItem(TextAsName);
		
		//Update Layout
		if (!PC->GetInvenWidget())	return false;
		PC->GetInvenWidget()->OnUpdateInvenUI.Broadcast();
		
		return true;
	}
	
	return false;
}

